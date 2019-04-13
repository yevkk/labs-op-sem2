using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;

namespace MessageLogWebApplication.Models
{
    public class Demo
    {
        private readonly MessageLogWebApplicationContext _context;
        public static Functions func;

        public Demo(MessageLogWebApplicationContext context)
        {
            _context = context;
            func = new Functions(context);
        }

        public string demoString = "";

        private void PrintMessages(IQueryable<Message> messages, int mode = 0)
        {
            demoString += "Messages:\n";
            if (mode == 0)
            {
                foreach (var m in messages)
                {
                    demoString += "ID: " + m.Id + '\n';
                    demoString += "Server ID: " + m.ServerId + '\n';
                    demoString += "Text: " + m.Text + '\n';
                    demoString += "Processing Date: " + m.ProcessingDate + '\n';
                    demoString += "TypeType: " + m.Type + '\n';
                    demoString += "Priority: " + m.Priority + '\n';
                    demoString += "Load Level: " + m.LoadLevel + "\n\n";
                }
            }
            else
            {
                int i = 1;
                foreach (var m in messages)
                {
                    demoString += i + ") " + m.Id + '\n';
                    i++;
                }
            }
        }

        private void PrintServers(IQueryable<Server> servers, int mode = 0)
        {
            demoString += "Servers:\n";
            if (mode == 0)
            {
                foreach (var s in servers)
                {
                    demoString += "ID: " + s.Id + '\n';
                    demoString += "Description: " + s.Description + '\n';
                    demoString += "IP: " + s.Ip + '\n';
                    demoString += "ReloadDate: " + s.ReloadDate + "\n\n";
                }
            }
            else
            {
                int i = 1;
                foreach (var s in servers)
                {
                    demoString += i + ") " + s.Id + '\n';
                    i++;
                }
            }

        }

        

        public void Main()
        {
            func.XmlCreate("wwwroot/xml/temp.xml");
            func.ClearServers(_context.Server);

            demoString += "Demo:\n\n";

            int num = func.random.Next(3, 7);
            for (int i = 1; i <= num; i++)
                _context.Add(func.GenerateRandomServer());
            demoString += num + " servers added;\n\n";
            _context.SaveChanges();

            num = func.random.Next(10, 20);
            for (int i = 0; i <= num; i++)
                _context.Add(func.GenerateRandomMessage());
            demoString += num + " messages added;\n\n";
            _context.SaveChanges();

            PrintServers(_context.Server);
            PrintMessages(_context.Message);

            string searchNameM = func.RandomString(2);
            demoString += "\nSearch messages by name \"" + searchNameM + "\":\n";
            PrintMessages(func.SearchMessages(searchNameM, null, null, null, ""), 1);

            int maxPr = func.random.Next(200);
            int minPr = func.random.Next(maxPr);
            demoString += "\nSearch messages by priority range: (from " + minPr + " to " + maxPr + "):\n";
            PrintMessages(func.SearchMessages("", null, minPr, maxPr, ""), 1);

            DateTime dateTimeM = func.RandomDateTime(DateTime.Today);
            demoString += "\nSearch messages by date: (after " + dateTimeM + "):\n";
            PrintMessages(func.SearchMessages("", dateTimeM, null, null, ""), 1);

            string searchNameS = func.RandomString(2);
            demoString += "\nSearch servers by name \"" + searchNameS + "\":\n";
            PrintServers(func.SearchServers(searchNameS, null, null, null), 1);

            DateTime dateTimeS2 = func.RandomDateTime(DateTime.Today);
            DateTime dateTimeS1 = func.RandomDateTime(dateTimeS2);
            float minLoadLvl = (float)func.random.NextDouble();
            demoString += "\nSearch servers by messages with date range (from " + dateTimeS1 + " to " + dateTimeS2 + ") and Load level higher than " + minLoadLvl + ":\n";
            PrintServers(func.SearchServers("", dateTimeS1, dateTimeS2, minLoadLvl), 1);

            demoString += "\nDeleting servers found earlier by messages;\n";
            func.ClearServers(func.SearchServers("", dateTimeS1, dateTimeS2, minLoadLvl));

            demoString += "\nDeleting messages found earlier by date;\n\n";
            func.ClearMessages(func.SearchMessages("", dateTimeM, null, null, ""));

            PrintServers(_context.Server);
            PrintMessages(_context.Message);

            demoString += "Saving data to .xml file;\n";
            func.XmlCreate("wwwroot/xml/demo.xml");

            demoString += "Saving data to .bin file;\n\n";
            func.BinCreate("wwwroot/dat/demo.bin");

            demoString += "Clearing all data, print:\n";
            func.ClearServers(_context.Server);
            PrintServers(_context.Server);
            PrintMessages(_context.Message);
            demoString += '\n';

            demoString += "Loading data from .xml file, print:\n";
            func.XmlLoad("wwwroot/xml/demo.xml");
            PrintServers(_context.Server);
            PrintMessages(_context.Message);

            demoString += "Clearing all data, print:\n";
            func.ClearServers(_context.Server);
            PrintServers(_context.Server);
            PrintMessages(_context.Message);
            demoString += '\n';

            demoString += "Loading data from .bin file, print:\n";
            func.BinLoad("wwwroot/dat/demo.bin");
            PrintServers(_context.Server);
            PrintMessages(_context.Message);

            demoString += "\nDemo finished;";

            StreamWriter sw = new StreamWriter(@"wwwroot/demo/result.txt", false);
            sw.Write(demoString);
            sw.Close();

            func.ClearServers(_context.Server);
            func.XmlLoad("wwwroot/xml/temp.xml");
        }

    }
}

