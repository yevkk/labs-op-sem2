using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace MessageWebApplication.Models
{
    public class Message
    {
        public int Id { get; set; }
        public int ServerId { get; set; }
        public string Text { get; set; }

        //[DataType(DataType.Date)]
        public DateTime DateTime { get; set; }
        public string Type { get; set; }
        public int Priority { get; set; }
        public double Load { get; set; }
    }
}
