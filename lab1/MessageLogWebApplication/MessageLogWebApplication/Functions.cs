using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Xml.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace MessageLogWebApplication.Models
{
    public class Functions
    {
        private readonly MessageLogWebApplicationContext _context;

        public Functions(MessageLogWebApplicationContext context)
        {
            _context = context;
        }

        public IQueryable<Message> SearchMessages(string searchString, DateTime? searchDate, int? minPriority, int? maxPriority, string searchType)
        {
            var messages = from m in _context.Message
                           select m;

            if (!string.IsNullOrEmpty(searchString))
            {
                messages = messages.Where(s => s.Text.Contains(searchString));
            }

            if (searchDate != null)
            {
                messages = messages.Where(x => x.ProcessingDate >= searchDate);
            }

            if (minPriority != null)
            {
                messages = messages.Where(s => s.Priority >= minPriority);
            }

            if (maxPriority != null)
            {
                messages = messages.Where(s => s.Priority <= maxPriority);
            }

            if (!string.IsNullOrEmpty(searchType))
            {
                messages = messages.Where(s => s.Type == searchType);
            }

            return messages;
        }

        public IQueryable<Server> SearchServers(string searchString, DateTime? firstDate, DateTime? secondDate, double? minLoad)
        {
            var servers = from m in _context.Server
                          select m;

            if (!string.IsNullOrEmpty(searchString))
            {
                servers = servers.Where(s => s.Description.Contains(searchString));
            }

            if (firstDate != null)
            {
                var messages = from m in _context.Message
                               select m;
                messages = messages.Where(m => m.ProcessingDate >= firstDate);
                servers = servers.Where(s => messages.Intersect(s.Message.AsQueryable()).Count() != 0);
            }

            if (secondDate != null)
            {
                var messages = from m in _context.Message
                               select m;
                messages = messages.Where(m => m.ProcessingDate <= secondDate);
                servers = servers.Where(s => messages.Intersect(s.Message.AsQueryable()).Count() != 0);
            }

            if (minLoad != null)
            {
                var messages = from m in _context.Message
                               select m;
                messages = messages.Where(m => m.LoadLevel >= minLoad);
                servers = servers.Where(s => messages.Intersect(s.Message.AsQueryable()).Count() != 0);
            }

            return servers;
        }

        public void XmlCreate(string filename)
        {
            XDocument xdoc = new XDocument();
            XElement xRoot = new XElement("root"); ;

            var messages = from m in _context.Message
                           select m;

            foreach (var m in messages)
            {
                XElement message = new XElement("message");

                XAttribute idAttr = new XAttribute("id", m.Id);
                XElement serverIdElem = new XElement("server_id", m.ServerId);
                XElement textElem = new XElement("text", m.Text);
                XElement processingDateElem = new XElement("processing_date", m.ProcessingDate);
                XElement typeElem = new XElement("type", m.Type);
                XElement priorityElem = new XElement("priority", m.Priority);
                XElement loadLevelElem = new XElement("load_level", m.LoadLevel);

                message.Add(idAttr);
                message.Add(serverIdElem);
                message.Add(textElem);
                message.Add(processingDateElem);
                message.Add(typeElem);
                message.Add(priorityElem);
                message.Add(loadLevelElem);

                xRoot.AddFirst(message);
            }

            var servers = from m in _context.Server
                          select m;

            foreach (var s in servers)
            {
                XElement server = new XElement("server");

                XAttribute idAttr = new XAttribute("id", s.Id);
                XElement descriptionElem = new XElement("description", s.Description);
                XElement ipElem = new XElement("ip", s.Ip);
                XElement reloadDateElem = new XElement("reload_date", s.ReloadDate);

                server.Add(idAttr);
                server.Add(descriptionElem);
                server.Add(ipElem);
                server.Add(reloadDateElem);

                xRoot.AddFirst(server);
            }

            xdoc.Add(xRoot);
            xdoc.Save(filename);
        }

        public void XmlLoad(string filename)
        {
            XDocument xdoc = XDocument.Load(filename);

            foreach (var message in _context.Message)
                _context.Message.Remove(message);

            foreach (var server in _context.Server)
                _context.Server.Remove(server);
            _context.SaveChanges();

            foreach (XElement serverElem in xdoc.Element("root").Elements("server"))
            {
                XAttribute idAttr = serverElem.Attribute("id");
                XElement descriptionElem = serverElem.Element("description");
                XElement ipElem = serverElem.Element("ip");
                XElement reloadDateElem = serverElem.Element("reload_date");

                if (descriptionElem != null && ipElem != null && reloadDateElem != null)
                {
                    Server server = new Server();
                    server.prevId = int.Parse(idAttr.Value);
                    server.Description = descriptionElem.Value;
                    server.Ip = ipElem.Value;
                    if (reloadDateElem.Value != "") server.ReloadDate = DateTime.Parse(reloadDateElem.Value);

                    _context.Add(server);
                }
            }
            _context.SaveChanges();

            foreach (XElement messageElem in xdoc.Element("root").Elements("message"))
            {
                //XAttribute idAttr = messageElem.Attribute("id");
                XElement serverIdElem = messageElem.Element("server_id");
                XElement textElem = messageElem.Element("text");
                XElement processingDateElem = messageElem.Element("processing_date");
                XElement typeElem = messageElem.Element("type");
                XElement priorityElem = messageElem.Element("priority");
                XElement loadLevelElem = messageElem.Element("load_level");


                if (serverIdElem != null && textElem != null && processingDateElem != null && typeElem != null && priorityElem != null && loadLevelElem != null)
                {
                    Message message = new Message();
                    message.Server = _context.Server.First(s => s.prevId == int.Parse(serverIdElem.Value));
                    message.ServerId = message.Server.Id;
                    message.Text = textElem.Value;
                    if (processingDateElem.Value != "") message.ProcessingDate = DateTime.Parse(processingDateElem.Value);
                    message.Type = typeElem.Value;
                    message.Priority = int.Parse(priorityElem.Value);
                    message.LoadLevel = float.Parse(loadLevelElem.Value);

                    _context.Add(message);
                }
            }
            _context.SaveChanges();
        }

        [Serializable]
        class SerializableDB
        {
            public List<Message> Message { get; set; }
            public List<Server> Server { get; set; }
            public SerializableDB(List<Message> messages, List<Server> servers)
            {
                Message = messages;
                Server = servers;
            }
        }

        public void BinCreate(string filename)
        {
            SerializableDB dB = new SerializableDB(_context.Message.ToList(), _context.Server.ToList());

            BinaryFormatter formatter = new BinaryFormatter();
            using (FileStream fs = new FileStream(filename, FileMode.OpenOrCreate))
            {
                formatter.Serialize(fs, dB);
            }

            dB.Server.Clear();
        }

        public void BinLoad(string filename)
        {
            foreach (var message in _context.Message)
                _context.Message.Remove(message);

            foreach (var server in _context.Server)
                _context.Server.Remove(server);
            _context.SaveChanges();

            BinaryFormatter formatter = new BinaryFormatter();
            using (FileStream fs = new FileStream(filename, FileMode.OpenOrCreate))
            {
                SerializableDB dB = (SerializableDB)formatter.Deserialize(fs);

                foreach (var s in dB.Server)
                {
                    Server server = new Server();
                    server.Description = s.Description;
                    server.Ip = s.Ip;
                    server.prevId = s.Id;

                    _context.Add(server);
                }
                _context.SaveChanges();

                foreach (var m in dB.Message)
                {
                    Message message = new Message();
                    message.Server = _context.Server.First(s => s.prevId == m.ServerId);
                    message.ServerId = message.Server.Id;
                    message.Text = m.Text;
                    if (m.ProcessingDate != null) message.ProcessingDate = m.ProcessingDate;
                    message.Type = m.Type;
                    message.Priority = m.Priority;
                    message.LoadLevel = m.LoadLevel;

                    _context.Add(message);
                }


            }

            _context.SaveChanges();
        }

        //private long Time(???)
        //{
        //    var watch = System.Diagnostics.Stopwatch.StartNew();
        //    // function();
        //    watch.Stop();

        //    return watch.ElapsedMilliseconds;
        //}

        //public long Benchmark(???)
        //{
        //    long n = 1;
        //    long time = 0;

        //    do
        //    {
        //        n *= 2;
        //        for (long i = 1; i <= n; i++)
        //            time = Time(theMethod, parametrs);
        //    } while (time < 10000);

        //    return n;
        //}


    }
}
