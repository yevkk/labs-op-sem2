using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace MessageLogWebApplication.Models
{
    public class HomeController : Controller
    {
        private readonly MessageLogWebApplicationContext _context;
        public static Functions func;

        public HomeController(MessageLogWebApplicationContext context)
        {
            _context = context;
            func = new Functions(context);
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Tools()
        {
            return View();
        }
               
        public IActionResult Demo()
        {
            return View();
        }

        public IActionResult Benchmark()
        {
            return View();
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }

        public IActionResult ClearContext()
        {
            func.ClearContext();
            return RedirectToAction(nameof(Tools));
        }

        public IActionResult XmlCreate()
        {
            func.XmlCreate("wwwroot/xml/messages.xml");
            return RedirectToAction(nameof(Tools));
        }


        public IActionResult XmlLoad()
        {
            func.XmlLoad("wwwroot/xml/messages.xml");
            return RedirectToAction(nameof(Tools));
        }

        public IActionResult BinCreate()
        {
            func.BinCreate("wwwroot/dat/messages.bin");
            return RedirectToAction(nameof(Tools));
        }

        public IActionResult BinLoad()
        {
            func.BinLoad("wwwroot/dat/messages.bin");
            return RedirectToAction(nameof(Tools));
        }

        public IActionResult BenchmarkResult() {
            //ViewData["Text"] = func.Benchmark();
            func.Benchmark();
            return View("Benchmark");
        }
    }
}
