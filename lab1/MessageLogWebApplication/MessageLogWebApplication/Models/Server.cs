using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;

namespace MessageLogWebApplication.Models
{
    [Serializable]
    public partial class Server
    {
        public Server()
        {
            Message = new HashSet<Message>();
        }

        public int Id { get; set; }
        
        public string Description { get; set; }

        [RegularExpression(@"^(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]).){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-925[0-5])$", ErrorMessage = "Please enter a valid value")]
        public string Ip { get; set; }

        public DateTime? ReloadDate { get; set; }

        public int? prevId = null;

        public ICollection<Message> Message { get; set; }
    }
}
