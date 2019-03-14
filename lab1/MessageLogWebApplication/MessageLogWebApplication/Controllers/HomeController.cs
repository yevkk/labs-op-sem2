using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using MessageLogWebApplication.Models;
using System.Xml.Linq;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;
using Microsoft.EntityFrameworkCore;

namespace MessageLogWebApplication.Controllers
{
    public class HomeController : Controller
    {
        private readonly MessageLogWebApplicationContext _context;

        public HomeController(MessageLogWebApplicationContext context)
        {
            _context = context;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Tools()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }

        public async Task<IActionResult> XmlCreate()
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
            xdoc.Save("wwwroot/xml/messages.xml");

            return RedirectToAction(nameof(Tools));
        }


        public async Task<IActionResult> XmlLoad()
        {
            XDocument xdoc = XDocument.Load("wwwroot/xml/messages.xml");

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

                    if (ModelState.IsValid)
                    {
                        _context.Add(server);
                    }

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

                    if (ModelState.IsValid)
                    {
                        _context.Add(message);
                    }

                }
            }
            _context.SaveChanges();
            return RedirectToAction(nameof(Tools));
        }

        [Serializable]
        class SerializableDB
        {
            public List<Message> Message { get; set; }
            public List<Server> Server { get; set; }
            public SerializableDB(List<Message> messages, List<Server> servers) {
                Message = messages;
                Server = servers;
            }
        }

        public async Task<IActionResult> BinCreate()
        {

            SerializableDB dB = new SerializableDB(_context.Message.ToList(), _context.Server.ToList());
           
            BinaryFormatter formatter = new BinaryFormatter();
            using (FileStream fs = new FileStream("wwwroot/bin/messages.bin", FileMode.OpenOrCreate))
            {
                formatter.Serialize(fs, dB);
            }

            dB.Server.Clear();
            return RedirectToAction(nameof(Tools));
        }

        public async Task<IActionResult> BinLoad()
        {
            foreach (var message in _context.Message)
                _context.Message.Remove(message);

            foreach (var server in _context.Server)
                _context.Server.Remove(server);
            _context.SaveChanges();

            BinaryFormatter formatter = new BinaryFormatter();
            using (FileStream fs = new FileStream("wwwroot/bin/messages.bin", FileMode.OpenOrCreate))
            {
                SerializableDB dB = (SerializableDB)formatter.Deserialize(fs);

                foreach (var s in dB.Server)
                {
                    Server server = new Server();
                    server.Description = s.Description;
                    server.Ip = s.Ip;
                    server.prevId = s.Id;
                    if (ModelState.IsValid)
                    {
                        _context.Add(server);
                    }
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
                    
                    if (ModelState.IsValid)
                    {
                        _context.Add(message);
                    }
                }


            }

            _context.SaveChanges();
            return RedirectToAction(nameof(Tools));
        }
    }
}
