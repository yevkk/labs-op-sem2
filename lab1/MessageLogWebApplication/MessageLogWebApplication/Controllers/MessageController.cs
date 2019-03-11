using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;
using System.Xml;
using System.Xml.Linq;
using System.IO;

namespace MessageLogWebApplication.Models
{
    public class MessageController : Controller
    {
        private readonly MessageLogWebApplicationContext _context;

        public MessageController(MessageLogWebApplicationContext context)
        {
            _context = context;
        }

        // GET: Message
       public async Task<IActionResult> Index(string searchString, DateTime? searchDate, int? minPriority, int? maxPriority, string searchType)
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


            return View(await messages.ToListAsync());
        }

        [HttpPost]
        public string Index(string searchString, bool notUsed)
        {
            return "From [HttpPost]Index: filter on " + searchString;
        }

        // GET: Message/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var message = await _context.Message
                .Include(m => m.Server)
                .FirstOrDefaultAsync(m => m.Id == id);
            if (message == null)
            {
                return NotFound();
            }

            return View(message);
        }

        // GET: Message/Create
        public IActionResult Create()
        {
            ViewData["ServerId"] = new SelectList(_context.Set<Server>(), "Id", "Description");
            return View();
        }

        // POST: Message/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("Id,ServerId,Text,ProcessingDate,Type,Priority,LoadLevel")] Message message)
        {
            if (ModelState.IsValid)
            {
                _context.Add(message);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            ViewData["ServerId"] = new SelectList(_context.Set<Server>(), "Id", "Id", message.ServerId);
            return View(message);
        }

        // GET: Message/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var message = await _context.Message.FindAsync(id);
            if (message == null)
            {
                return NotFound();
            }
            ViewData["ServerId"] = new SelectList(_context.Set<Server>(), "Id", "Description", message.ServerId);
            return View(message);
        }

        // POST: Message/Edit/5
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("Id,ServerId,Text,ProcessingDate,Type,Priority,LoadLevel")] Message message)
        {
            if (id != message.Id)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(message);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!MessageExists(message.Id))
                    {
                        return NotFound();
                    }
                    else
                    {
                        throw;
                    }
                }
                return RedirectToAction(nameof(Index));
            }
            ViewData["ServerId"] = new SelectList(_context.Set<Server>(), "Id", "Id", message.ServerId);
            return View(message);
        }

        // GET: Message/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var message = await _context.Message
                .Include(m => m.Server)
                .FirstOrDefaultAsync(m => m.Id == id);
            if (message == null)
            {
                return NotFound();
            }

            return View(message);
        }

        // POST: Message/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            var message = await _context.Message.FindAsync(id);
            _context.Message.Remove(message);
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool MessageExists(int id)
        {
            return _context.Message.Any(e => e.Id == id);
        }

        public void XmlCreate() {
            XmlDocument xdoc = new XmlDocument();
            //xdoc.Load("~/images/messages.xml");
            XmlElement xRoot = xdoc.CreateElement("messages"); ;

            var messages = from m in _context.Message
                           select m;

            foreach (var m in messages) {
                XmlElement message = xdoc.CreateElement("message");
                XmlAttribute messageIdAttr = xdoc.CreateAttribute("id");
                XmlElement messageServerIdElem = xdoc.CreateElement("server_id");
                XmlElement messageTextElem = xdoc.CreateElement("text");
                XmlElement messageProcessingDateElem = xdoc.CreateElement("processing_date");
                XmlElement messageTypeElem = xdoc.CreateElement("type");
                XmlElement messagePriorityElem = xdoc.CreateElement("priority");
                XmlElement messageLoadLevelElem = xdoc.CreateElement("load_level");

                XmlText idText = xdoc.CreateTextNode(m.Id.ToString());
                XmlText serverIdText = xdoc.CreateTextNode(m.ServerId.ToString());
                XmlText textText = xdoc.CreateTextNode(m.Text);
                XmlText processingDateText = xdoc.CreateTextNode(m.ProcessingDate.ToString());
                XmlText typeText = xdoc.CreateTextNode(m.Type);
                XmlText priorityText = xdoc.CreateTextNode(m.Priority.ToString());
                XmlText loadLevelText = xdoc.CreateTextNode(m.LoadLevel.ToString());

                messageIdAttr.AppendChild(idText);
                messageServerIdElem.AppendChild(serverIdText);
                messageTextElem.AppendChild(textText);
                messageProcessingDateElem.AppendChild(processingDateText);
                messageTypeElem.AppendChild(typeText);
                messagePriorityElem.AppendChild(priorityText);
                messageLoadLevelElem.AppendChild(loadLevelText);

                message.Attributes.Append(messageIdAttr);
                message.AppendChild(messageServerIdElem);
                message.AppendChild(messageTextElem);
                message.AppendChild(messageProcessingDateElem);
                message.AppendChild(messageTypeElem);
                message.AppendChild(messagePriorityElem);
                message.AppendChild(messageLoadLevelElem);

                xRoot.AppendChild(message);
            }
            xdoc.AppendChild(xRoot);
            xdoc.Save("wwwroot/Xml/messages.xml");
        }

       

    }
}
