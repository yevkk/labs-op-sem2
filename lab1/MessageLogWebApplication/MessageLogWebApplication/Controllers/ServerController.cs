using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Rendering;
using Microsoft.EntityFrameworkCore;

namespace MessageLogWebApplication.Models
{
    public class ServerController : Controller
    {
        private readonly MessageLogWebApplicationContext _context;

        public ServerController(MessageLogWebApplicationContext context)
        {
            _context = context;
        }

        // GET: Server
        public async Task<IActionResult> Index(string searchString, DateTime? firstDate, DateTime? secondDate, double? minLoad)
        {
            IQueryable<Server> servers = Search(searchString, firstDate, secondDate, minLoad);

            return View(await servers.ToListAsync());
        }

        private IQueryable<Server> Search(string searchString, DateTime? firstDate, DateTime? secondDate, double? minLoad)
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


        // GET: Server/Details/5
        public async Task<IActionResult> Details(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var server = await _context.Server
                .FirstOrDefaultAsync(m => m.Id == id);
            if (server == null)
            {
                return NotFound();
            }

            return View(server);
        }

        // GET: Server/Create
        public IActionResult Create()
        {
            return View();
        }

        // POST: Server/Create
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Create([Bind("Id,Description,Ip,ReloadDate")] Server server)
        {
            if (ModelState.IsValid)
            {
                _context.Add(server);
                await _context.SaveChangesAsync();
                return RedirectToAction(nameof(Index));
            }
            return View(server);
        }

        // GET: Server/Edit/5
        public async Task<IActionResult> Edit(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            var server = await _context.Server.FindAsync(id);
            if (server == null)
            {
                return NotFound();
            }
            return View(server);
        }

        // POST: Server/Edit/5
        // To protect from overposting attacks, please enable the specific properties you want to bind to, for 
        // more details see http://go.microsoft.com/fwlink/?LinkId=317598.
        [HttpPost]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> Edit(int id, [Bind("Id,Description,Ip,ReloadDate")] Server server)
        {
            if (id != server.Id)
            {
                return NotFound();
            }

            if (ModelState.IsValid)
            {
                try
                {
                    _context.Update(server);
                    await _context.SaveChangesAsync();
                }
                catch (DbUpdateConcurrencyException)
                {
                    if (!ServerExists(server.Id))
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
            return View(server);
        }

        // GET: Server/Delete/5
        public async Task<IActionResult> Delete(int? id)
        {
            if (id == null)
            {
                return NotFound();
            }

            foreach (var message in _context.Message)
                if (message.ServerId == id) _context.Message.Remove(message);
            _context.SaveChanges();

            var server = await _context.Server
                .FirstOrDefaultAsync(m => m.Id == id);
            if (server == null)
            {
                return NotFound();
            }

            return View(server);
        }

        // POST: Server/Delete/5
        [HttpPost, ActionName("Delete")]
        [ValidateAntiForgeryToken]
        public async Task<IActionResult> DeleteConfirmed(int id)
        {
            var server = await _context.Server.FindAsync(id);
            _context.Server.Remove(server);
            await _context.SaveChangesAsync();
            return RedirectToAction(nameof(Index));
        }

        private bool ServerExists(int id)
        {
            return _context.Server.Any(e => e.Id == id);
        }
    }
}
