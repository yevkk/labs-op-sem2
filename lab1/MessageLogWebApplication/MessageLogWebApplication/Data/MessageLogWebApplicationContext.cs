using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;
using MessageLogWebApplication.Models;

namespace MessageLogWebApplication.Models
{
    public class MessageLogWebApplicationContext : DbContext
    {
        public MessageLogWebApplicationContext (DbContextOptions<MessageLogWebApplicationContext> options)
            : base(options)
        {
        }

        public DbSet<MessageLogWebApplication.Models.Message> Message { get; set; }

        public DbSet<MessageLogWebApplication.Models.Server> Server { get; set; }
    }
}
