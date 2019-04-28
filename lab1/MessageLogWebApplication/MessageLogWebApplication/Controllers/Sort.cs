using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Dynamic.Core;

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
            messageList = messages.ToList();
            int n = messageList.Count();

            for (int i = 0; i < n - 1; i++)
                for (int j = 0; j < n - i - 1; j++)
                    if (messageList[j].Priority > messageList[j + 1].Priority)
                        SwapInList(ref messageList, j, j + 1);


            return messageList;
        }

        private bool MergeFlag(List<Message> messageList, int i1, int i2, string mode)
        {
            switch (mode)
            {
                case "serverId":
                    {
                        return (messageList[i1].ServerId < messageList[i2].ServerId);
                    }
                case "text":
                    {
                        return (string.Compare(messageList[i1].Text, messageList[i2].Text) < 0) ? true : false;
                    }
                case "processingDate":
                    {
                        return (messageList[i1].ProcessingDate < messageList[i2].ProcessingDate);
                    }
                case "type":
                    {
                        return (string.Compare(messageList[i1].Type, messageList[i2].Type) < 0) ? true : false;
                    }
                case "priority":
                    {
                        return (messageList[i1].Priority < messageList[i2].Priority);
                    }
                case "loadLevel":
                    {
                        return (messageList[i1].LoadLevel < messageList[i2].LoadLevel);
                    }
            }
            return true;
        }

        private List<Message> Merge(List<Message> messageList, int left, int mid, int right, string mode)
        {
            List<Message> result = new List<Message>();
            int p1 = 0, p2 = 0;
            Message m = new Message();

            while ((left + p1 < mid) && (mid + p2 < right))
            {
                if (MergeFlag(messageList, left + p1, mid + p2, mode))
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

        public List<Message> MergeSort(IQueryable<Message> messages, string mode)
        {
            List<Message> messageList = messages.ToList();
            int n = messageList.Count();

            for (int i = 1; i < n; i *= 2)
                for (int j = 0; j <= n - i; j += 2 * i)
                    messageList = Merge(messageList, j, j + i, Math.Min(j + 2 * i, n), mode);

            return messageList;
        }

        public List<Message> CountingSort(IQueryable<Message> messages) //sort by 'type' property;
        {
            List<Message> messageList = messages.ToList();
            List<Message> newList = new List<Message>();
            List<int>[] arr = new List<int>[5];

            for (int i = 0; i < 5; i++)
            {
                arr[i] = new List<int>();
            }

            int j = 0;
            for (int i = 0; i < messageList.Count(); i++)
            {
                j = 0;
                while (true)
                {
                    if (arr[j].Count() == 0)
                    {
                        arr[j].Add(i);
                        break;
                    }
                    else if (messageList[arr[j][0]].Type == messageList[i].Type)
                    {
                        arr[j].Add(i);
                        break;
                    }
                    else
                    {
                        j++;
                    }
                }

            }

            for (int i = 0; i < 5; i++)
            {
                foreach (int k in arr[i])
                {
                    newList.Add(messageList[k]);
                }
            }
            //Message msg = new Message
            //{
            //    Priority = arr[0].Count(),
            //    Type = messageList[0].Type
            //};
            //newList.Add(msg);
            return newList;
        }

        public IQueryable<Message> SortGG(IQueryable<Message> messages, string queryString)
        {
            return messages.OrderBy(queryString);
        }

    }
}
