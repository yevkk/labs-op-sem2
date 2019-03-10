using System;
using System.Collections.Generic;

namespace MessageLogWebApplication.Models
{
    public partial class Server
    {
        public Server()
        {
            Message = new HashSet<Message>();
        }

        public int Id { get; set; }
        public string Description { get; set; }
        public string Ip { get; set; }
        public DateTime? ReloadDate { get; set; }

        public ICollection<Message> Message { get; set; }
    }
}
