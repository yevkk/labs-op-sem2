using Microsoft.AspNetCore.Mvc.Rendering;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace MessageLogWebApplication.Models
{
    [Serializable]
    public partial class Message
    {
        public int Id { get; set; }
        public int ServerId { get; set; }
        public string Text { get; set; }
        public DateTime? ProcessingDate { get; set; }
        public string Type { get; set; }

        [Range(0, 200)]
        public int? Priority { get; set; }

        [Range(0, 1)]
        public float? LoadLevel { get; set; }

        public Server Server { get; set; }
        
    }
}
