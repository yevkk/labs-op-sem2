using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using System.Diagnostics;

namespace MessageLogWebApplication.Models
{
    public class Functions
    {
        private readonly MessageLogWebApplicationContext _context;

        public Functions(MessageLogWebApplicationContext context)
        {
            _context = context;
        }

        public void ClearMessages(IQueryable<Message> messages) {
            foreach (var message in messages)
                _context.Message.Remove(message);
            _context.SaveChanges();
        }

        public void ClearServers(IQueryable<Server> servres) // with _context.Server argument method is equal to clearing all data, because messages cannot exist without servers;
        {
            foreach (var server in servres) {
                foreach (var message in _context.Message)
                    if (message.ServerId == server.Id) _context.Message.Remove(message);
                _context.Server.Remove(server);
            }
            _context.SaveChanges();
        }

        public Random random = new Random();
        public string RandomString(int length)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            return new string(Enumerable.Repeat(chars, length)
              .Select(s => s[random.Next(s.Length)]).ToArray());
        }
        public DateTime RandomDateTime(DateTime max)
        {
            DateTime start = new DateTime(2000, 1, 1);
            int range = (max - start).Days;
            return start.AddDays(random.Next(range));
        }

        List<string> TypeList = new List<string>()
            {
                "debug",
                "info",
                "warning",
                "error",
                "fatal"
            };

        public Server GenerateRandomServer()
        {
            Server server = new Server
            {
                Description = RandomString(random.Next(5, 20)),
                Ip = random.Next(256).ToString() + "." + random.Next(256).ToString() + "." + random.Next(256).ToString() + "." + random.Next(256).ToString(),
                ReloadDate = RandomDateTime(DateTime.Today)
            };
            return server;
        }

        public Message GenerateRandomMessage()
        {
            List<int> ServerIdList = new List<int>();
            foreach (var s in _context.Server)
                ServerIdList.Add(s.Id);
            if (ServerIdList.Count() > 0)
            {
                Message message = new Message
                {
                    ServerId = ServerIdList[random.Next(ServerIdList.Count)],
                    Text = RandomString(random.Next(5, 50)),
                    Type = TypeList[random.Next(TypeList.Count)],
                    Priority = random.Next(200),
                    LoadLevel = (float)random.NextDouble()
                };
                message.Server = _context.Server.First(s => s.Id == message.ServerId);
                message.ProcessingDate = RandomDateTime(DateTime.Today);
                if (message.ProcessingDate > message.Server.ReloadDate) message.ProcessingDate = message.Server.ReloadDate;

                return message;
            }
            return null;
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

            ClearServers(_context.Server);

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
            ClearServers(_context.Server);

            BinaryFormatter formatter = new BinaryFormatter();
            using (FileStream fs = new FileStream(filename, FileMode.OpenOrCreate))
            {
                SerializableDB dB = (SerializableDB)formatter.Deserialize(fs);

                foreach (var s in dB.Server)
                {
                    Server server = new Server
                    {
                        Description = s.Description,
                        Ip = s.Ip,
                        prevId = s.Id
                    };

                    _context.Add(server);
                }
                _context.SaveChanges();

                foreach (var m in dB.Message)
                {
                    Message message = new Message
                    {
                        Server = _context.Server.First(s => s.prevId == m.ServerId),
                        Type = m.Type,
                        Priority = m.Priority,
                        LoadLevel = m.LoadLevel,
                        Text = m.Text
                    };
                    message.ServerId = message.Server.Id;
                    if (m.ProcessingDate != null) message.ProcessingDate = m.ProcessingDate;
                    

                    _context.Add(message);
                }


            }

            _context.SaveChanges();
        }

        public long Time(int n = 100) {
            ClearServers(_context.Server);
            Stopwatch sw = new Stopwatch();
            sw.Start();

            for (int i = 1; i <= n; i++)
                _context.Add(GenerateRandomServer());
            _context.SaveChanges();
            for (int i = 1; i <= n; i++)
                _context.Add(GenerateRandomMessage());
            _context.SaveChanges();

            XmlCreate("wwwroot/xml/demo.xml");
            BinCreate("wwwroot/dat/demo.bin");
            XmlLoad("wwwroot/xml/demo.xml");
            BinLoad("wwwroot/dat/demo.bin");
            int priority = random.Next(200);
            DateTime date2 = RandomDateTime(DateTime.Today);
            DateTime date1 = RandomDateTime(date2);
            SearchMessages(RandomString(random.Next(5, 10)), RandomDateTime(DateTime.Today), priority, random.Next(priority), TypeList[random.Next(TypeList.Count)]);
            SearchServers(RandomString(random.Next(5, 10)), date2, date1, (float)random.NextDouble());

            sw.Stop();
            return sw.ElapsedMilliseconds;
        }

        public List<string> Benchmark(int sec) // 0 - time ms; 1 - N; 3 - data size; 
        {
            XmlCreate("wwwroot/xml/temp.xml");
            ClearServers(_context.Server);

            List<string> results = new List<string>();
            int n = 1;
            long time = 0;

            while (time < sec * 1000)
            {
                time = Time(n);
                if (time < sec * 100)
                    n *= 2;
                else n += 20;
            }
            results.Add("N: " + n.ToString());
            results.Add("Time (ms): " + time.ToString());

            SerializableDB dB = new SerializableDB(_context.Message.ToList(), _context.Server.ToList());
            BinaryFormatter bf = new BinaryFormatter();
            MemoryStream ms = new MemoryStream();
            FileInfo xmlFile = new FileInfo("wwwroot/xml/demo.xml");
            FileInfo binFile = new FileInfo("wwwroot/dat/demo.bin");

            bf.Serialize(ms, dB);
            results.Add("Size (bytes): " + (ms.Length + xmlFile.Length + binFile.Length).ToString());

            ClearServers(_context.Server);

            XmlLoad("wwwroot/xml/temp.xml");

            return results;
        }




    }
}
