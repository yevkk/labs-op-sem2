using System;
using System.Collections.Generic;
using System.Linq;

namespace MessageLogWebApplication.Models
{
    public class Sort
    {
        private readonly MessageLogWebApplicationContext _context;

        public Sort(MessageLogWebApplicationContext context)
        {
            _context = context;
        }

        private void SwapInList(ref List<Message> messages, int i, int j)
        {
            Message m = messages[i];
            messages[i] = messages[j];
            messages[j] = m;
        }

        public List<Message> BubbleSort(IQueryable<Message> messages)
        {
            List<Message> messageList = new List<Message>();

            //foreach (var m in messages)
            //    messageList.Add(m);
            messageList = messages.ToList();
            int n = messageList.Count();

            for (int i = 0; i < n - 1; i++)
                for (int j = 0; j < n - i - 1; j++)
                    if (messageList[j].Priority > messageList[j + 1].Priority)
                        SwapInList(ref messageList, j, j + 1);


            return messageList;
        }

        private List<Message> Merge(List<Message> messageList, int left, int mid, int right)
        {
            List<Message> result = new List<Message>();
            int p1 = 0, p2 = 0;
            Message m = new Message();

            while ((left + p1 < mid) && (mid + p2 < right))
            {
                if (messageList[left + p1].Priority < messageList[mid + p2].Priority)
                {
                    m = messageList[left + p1];
                    result.Add(m);
                    p1++;
                }
                else
                {
                    m = messageList[mid + p2];
                    result.Add(m);
                    p2++;
                }
            }

            while (left + p1 < mid)
            {
                m = messageList[left + p1];
                result.Add(m);
                p1++;
            }

            while (mid + p2 < right)
            {
                m = messageList[mid + p2];
                result.Add(m);
                p2++;
            }

            for (int i = 0; i < p1 + p2; i++)
                messageList[left + i] = result[i];

            return messageList;
        }

        public List<Message> MergeSort(IQueryable<Message> messages)
        {
            List<Message> messageList = messages.ToList();
            int n = messageList.Count();

            for (int i = 1; i < n; i *= 2)
                for (int j = 0; j <= n - i; j += 2 * i)
                    messageList = Merge(messageList, j, j + i, Math.Min(j + 2 * i, n));

            return messageList;
        }

    }
}
