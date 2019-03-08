using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.EntityFrameworkCore;

namespace MessageWebApplication.Models
{
    public class MessageWebApplicationContext : DbContext
    {
        public MessageWebApplicationContext (DbContextOptions<MessageWebApplicationContext> options)
            : base(options)
        {
        }

        public DbSet<MessageWebApplication.Models.Message> Message { get; set; }
    }
}
