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

        //-------------------------------------

        private void MergeSortedParts(ref List<Message> src, ref List<Message> dst, int first, int middle, int size)
        {
            int i = first, j = middle;

            for (int k = first; k < size; k++)
            {
                if ((i < middle) && ((j >= size) || (src[i].Priority < src[j].Priority)))
                {
                    dst[k] = src[i];
                    i++;
                }
                else
                {
                    dst[k] = src[j];
                    j++;
                }

            }
        }

        private void SplitForSort(ref List<Message> src, ref List<Message> dst, int first, int size)
        {
            if (size - first < 2) return;

            int middle = (first + size) / 2;

            SplitForSort(ref dst, ref src, first, middle);
            SplitForSort(ref dst, ref src, middle, size);

            MergeSortedParts(ref src, ref dst, first, middle, size);
        }

        public List<Message> MergeSort(IQueryable<Message> messages) //top-down implementation;
        {
            List<Message> messageList = messages.ToList();
            List<Message> newList = messageList;
            SplitForSort(ref newList, ref messageList, 0, messages.Count());
            return newList;
        }


    }
}
