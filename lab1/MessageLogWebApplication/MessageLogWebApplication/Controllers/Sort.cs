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

        public List<Message> QuickSort(IQueryable<Message> messages) // lomuto, rightmost pivot
        {
            List<Message> messageList = new List<Message>();

            foreach (var m in messages)
                messageList.Add(m);

            QuickSortRec(ref messageList, 0, (messageList.Count() - 1));
            return messageList;
        }

        private int QSPartition(ref List<Message> messages, int first, int last)
        {
            int pivot = (int)messages[last].Priority;
            int i = first;

            for (int j = first; j <= last - 1; j++)
            {
                if (messages[j].Priority <= pivot)
                {
                    SwapInList(ref messages, i, j);
                    i++;
                }
            }
            SwapInList(ref messages, i, last);

            return i;
        }

        private void QuickSortRec(ref List<Message> messages, int first, int last)
        {
            if (first < last)
            {
                int p = QSPartition(ref messages, first, last);
                QuickSortRec(ref messages, first, p);
                QuickSortRec(ref messages, p + 1, last);
            }
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



    }
}
