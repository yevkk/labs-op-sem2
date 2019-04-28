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
        public static Demo demo;
        

        public HomeController(MessageLogWebApplicationContext context)
        {
            _context = context;
            func = new Functions(context);
            demo = new Demo(context);
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

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }

        public IActionResult ClearMessages()
        {
            func.ClearMessages(_context.Message);
            return RedirectToAction(nameof(Tools));
        }

        public IActionResult ClearServers()
        {
            func.ClearServers(_context.Server);
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

        public IActionResult BenchmarkResult(int sec) {
            if (sec != 0)
                ViewBag.Benchmark = func.Benchmark(sec);
            else
            {
                ViewBag.Benchmark = new List<string>() { "---", "---", "---" };
            }
            return View("Benchmark");
        }

        public IActionResult SortBenchmarkResult(int num)
        {
            if (num != 0)
                ViewBag.Benchmark = func.SortBenchmark(num);
            else
            {
                ViewBag.Benchmark = new List<string>() { "---", "---", "---" };
            }
            return View("SortBenchmark");
        }

        public IActionResult DemoResult()
        {
            demo.Main();
            return View("Demo");
        }
    }
}
