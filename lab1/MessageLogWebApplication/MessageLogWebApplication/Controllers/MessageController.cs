using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;

namespace MessageLogWebApplication.Models
{
    public class MessageController : Controller
    {
        private readonly MessageLogWebApplicationContext _context;
        private readonly Functions func;
        public static Sort sort;

        public MessageController(MessageLogWebApplicationContext context)
        {
            _context = context;
            func = new Functions(context);
            sort = new Sort(context);
        }



        // GET: Message
        public async Task<IActionResult> Index(string searchString, DateTime? searchDate, int? minPriority, int? maxPriority, string searchType, string SortType, string QSMode,
            string LSProp1, string LSProp2, string LSProp3, string LSProp4, string LSProp5, string LSProp6, string[] LSOrd)
        {
            ViewBag.Types = new SelectList(func.TypeList.AsEnumerable());
            ViewBag.Properties = new SelectList(func.PropertyList.AsEnumerable());
            IQueryable<Message> messages = func.SearchMessages(searchString, searchDate, minPriority, maxPriority, searchType);
            List<string> LSProp = new List<string>() {
                LSProp1, LSProp2, LSProp3, LSProp4, LSProp5, LSProp6,
            };
            switch (SortType)
            {
                case "MS":
                    {
                        messages = sort.MergeSort(messages, QSMode).AsQueryable();
                        break;
                    }
                case "CS":
                    {
                        messages = sort.CountingSort(messages).AsQueryable();
                        break;
                    }
                case "RS":
                    {
                        messages = sort.RadixSort(messages).AsQueryable();
                        break;
                    }
                case "LS":
                    {
                        string queryString = "";
                        for (int i = 0; i < 6; i++) {
                            if (LSProp[i] == "---") break;
                            else
                            {
                                if (i != 0) queryString += ", ";
                                queryString += LSProp[i] + " " + LSOrd[i];
                            }
                        }                          
                        if (queryString == "") break;
                        messages = sort.LinqSort(messages, queryString).AsQueryable();
                        break;
                    }
            }
            
            return View(messages);

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
            ViewBag.Types = new SelectList(func.TypeList.AsEnumerable());
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
            ViewBag.Types = new SelectList(func.TypeList.AsEnumerable());
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
            ViewBag.Types = new SelectList(func.TypeList.AsEnumerable());
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
            ViewBag.Types = new SelectList(func.TypeList.AsEnumerable());
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

        public async Task<IActionResult> CreateRandom()
        {
            Message message = func.GenerateRandomMessage();
            if (message != null)
            {
                _context.Add(message);
                await _context.SaveChangesAsync();
            }
            return RedirectToAction(nameof(Index));
        }
    }
}
