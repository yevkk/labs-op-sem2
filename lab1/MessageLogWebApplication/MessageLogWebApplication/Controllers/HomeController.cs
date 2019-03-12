using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using MessageLogWebApplication.Models;
using System.Xml.Linq;


namespace MessageLogWebApplication.Controllers
{
    public class HomeController : Controller
    {
        private readonly MessageLogWebApplicationContext _context;

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult About()
        {
            ViewData["Message"] = "Your application description page.";

            return View();
        }

        public IActionResult Contact()
        {
            ViewData["Message"] = "Your contact page.";

            return View();
        }

        public IActionResult Privacy()
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
            XElement xRoot = new XElement("messages"); ;

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
            xdoc.Add(xRoot);
            xdoc.Save("wwwroot/Xml/messages.xml");

            return RedirectToAction(nameof(Index));
        }


        public async Task<IActionResult> LoadXml()
        {
            XDocument xdoc = XDocument.Load("wwwroot/Xml/messages.xml");

            foreach (var message in _context.Message)
                _context.Message.Remove(message);
            _context.SaveChanges();

            foreach (XElement messageElem in xdoc.Element("messages").Elements("message"))
            {
                XAttribute idAttr = messageElem.Attribute("id");
                XElement serverIdElem = messageElem.Element("server_id");
                XElement textElem = messageElem.Element("text");
                XElement processingDateElem = messageElem.Element("processing_date");
                XElement typeElem = messageElem.Element("type");
                XElement priorityElem = messageElem.Element("priority");
                XElement loadLevelElem = messageElem.Element("load_level");


                if (idAttr != null && serverIdElem != null && textElem != null && processingDateElem != null && typeElem != null && priorityElem != null && loadLevelElem != null)
                {
                    Message message = new Message();
                    message.ServerId = int.Parse(serverIdElem.Value);
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
            return RedirectToAction(nameof(Index));
        }
    }
}
