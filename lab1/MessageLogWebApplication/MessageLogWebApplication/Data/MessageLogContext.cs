using System;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Metadata;

namespace MessageLogWebApplication.Models
{
    public partial class MessageLogContext : DbContext
    {
        public MessageLogContext()
        {
        }

        public MessageLogContext(DbContextOptions<MessageLogContext> options)
            : base(options)
        {
        }

        public virtual DbSet<Message> Message { get; set; }
        public virtual DbSet<Server> Server { get; set; }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            if (!optionsBuilder.IsConfigured)
            {
#warning To protect potentially sensitive information in your connection string, you should move it out of source code. See http://go.microsoft.com/fwlink/?LinkId=723263 for guidance on storing connection strings.
                optionsBuilder.UseSqlServer("Server=DESKTOP-U80E832;Database=Message Log;Trusted_Connection=True;");
            }
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Message>(entity =>
            {
                entity.Property(e => e.Id).HasColumnName("ID");

                entity.Property(e => e.ProcessingDate).HasColumnType("datetime");

                entity.Property(e => e.ServerId).HasColumnName("ServerID");

                entity.Property(e => e.Type).HasMaxLength(10);

                entity.HasOne(d => d.Server)
                    .WithMany(p => p.Message)
                    .HasForeignKey(d => d.ServerId)
                    .OnDelete(DeleteBehavior.ClientSetNull)
                    .HasConstraintName("FK_Message_Server");
            });

            modelBuilder.Entity<Server>(entity =>
            {
                entity.Property(e => e.Id).HasColumnName("ID");

                entity.Property(e => e.Ip)
                    .HasColumnName("IP")
                    .HasMaxLength(50);

                entity.Property(e => e.ReloadDate).HasColumnType("datetime");
            });
        }
    }
}
