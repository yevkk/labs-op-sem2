using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Graphs
{
    public partial class MainForm : Form
    {
        private Graph _graph = new Graph();
        private const int RADIUS = 20;
        private Color _color = Color.Black;
        private Pen _blackPen = new Pen(Color.Black, 2);
        private string _action = "nodeAddBtn";
        private const byte MAXNODE = 100;
        private byte[,] _matrixArray = new byte[MAXNODE, MAXNODE];
        private int _selectedNode = -1;
        private int[] queue = new int[MAXNODE];
        private int bfsHead = 0;
        private int bfsTail = 1;
        private bool searchEnd = false;
        private int nodeNum;
        private int queueSize;
        private int v;
        private bool isConnected = true;



        public MainForm()
        {
            InitializeComponent();
            DrawGraph();
        }

        public void DrawGraph()
        {
            matrixTB.Text = "";
            Bitmap bitmap = new Bitmap(canvas.Width, canvas.Height);
            canvas.Image = bitmap;
            Graphics graphics = Graphics.FromImage(bitmap);

            graphics.Clear(Color.SeaShell);

            foreach (var edge in _graph.Edges)
            {
                graphics.DrawLine(_blackPen, _graph.Nodes[edge.N1].X, _graph.Nodes[edge.N1].Y, _graph.Nodes[edge.N2].X, _graph.Nodes[edge.N2].Y);
            }

            foreach (var node in _graph.Nodes)
            {
                graphics.DrawEllipse(new Pen(node.Cl, 3), node.X - RADIUS, node.Y - RADIUS, 2 * RADIUS, 2 * RADIUS);
                graphics.FillEllipse(Brushes.White, node.X - RADIUS, node.Y - RADIUS, 2 * RADIUS, 2 * RADIUS);
                /**/
                graphics.DrawString((_graph.Nodes.IndexOf(node) + 1).ToString(), new Font("Consolas", 15), Brushes.Black, node.X - RADIUS * _graph.Nodes.IndexOf(node).ToString().Length / 3 + 1, node.Y - RADIUS / 2 + 1);

                if (node.Slc == true)
                {
                    graphics.DrawEllipse(new Pen(Color.Red, 5), node.X - RADIUS, node.Y - RADIUS, 2 * RADIUS, 2 * RADIUS);
                    graphics.FillEllipse(Brushes.White, node.X - RADIUS, node.Y - RADIUS, 2 * RADIUS, 2 * RADIUS);
                    /**/
                    graphics.DrawString((_graph.Nodes.IndexOf(node) + 1).ToString(), new Font("Consolas", 15), Brushes.Red, node.X - RADIUS * _graph.Nodes.IndexOf(node).ToString().Length / 3 + 1, node.Y - RADIUS / 2 + 1);
                }
            }
            if (_selectedNode != -1)
            {
                graphics.DrawEllipse(new Pen(Color.Red, 5), _graph.Nodes[_selectedNode].X - RADIUS, _graph.Nodes[_selectedNode].Y - RADIUS, 2 * RADIUS, 2 * RADIUS);
                graphics.FillEllipse(Brushes.White, _graph.Nodes[_selectedNode].X - RADIUS, _graph.Nodes[_selectedNode].Y - RADIUS, 2 * RADIUS, 2 * RADIUS);
                /**/
                graphics.DrawString((_graph.Nodes.IndexOf(_graph.Nodes[_selectedNode]) + 1).ToString(), new Font("Consolas", 15), Brushes.Red, _graph.Nodes[_selectedNode].X - RADIUS * _graph.Nodes.IndexOf(_graph.Nodes[_selectedNode]).ToString().Length / 3 + 1, _graph.Nodes[_selectedNode].Y - RADIUS / 2 + 1);
            }

            for (int i = 0; i < MAXNODE; i++)
            {
                for (int j = 0; j < MAXNODE; j++)
                {
                    _matrixArray[i, j] = 0;
                }
            }

            foreach (var edge in _graph.Edges)
            {
                _matrixArray[edge.N1, edge.N2] = 1;
                _matrixArray[edge.N2, edge.N1] = 1;
            }

        }

        private void Canvas_MouseClick(object sender, MouseEventArgs e)
        {
            switch (_action)
            {
                case "nodeAddBtn":
                    foreach (var node in _graph.Nodes)
                    {
                        if ((Math.Pow((node.X - e.X), 2) + Math.Pow((node.Y - e.Y), 2)) < 4 * RADIUS * RADIUS)
                        {
                            return;
                        }
                    }

                    _graph.Nodes.Add(new Node(e.X, e.Y, _color, false));
                    DrawGraph();
                    break;
                case "nodeDestroyBtn":
                    foreach (var node in _graph.Nodes)
                    {
                        if ((Math.Pow((node.X - e.X), 2) + Math.Pow((node.Y - e.Y), 2)) < RADIUS * RADIUS)
                        {
                            _graph.Edges = (from edge in _graph.Edges
                                            where (edge.N1 != _graph.Nodes.IndexOf(node)) &&
                                            (edge.N2 != _graph.Nodes.IndexOf(node))
                                            select edge).ToList();

                            foreach (var edge in _graph.Edges)
                            {
                                if (edge.N1 > _graph.Nodes.IndexOf(node))
                                {
                                    edge.N1--;
                                }
                                if (edge.N2 > _graph.Nodes.IndexOf(node))
                                {
                                    edge.N2--;
                                }
                            }

                            _graph.Nodes.Remove(node);
                            break;
                        }
                    }

                    DrawGraph();
                    break;
                case "edgeAddBtn":
                    foreach (var node in _graph.Nodes)
                    {
                        if ((Math.Pow((node.X - e.X), 2) + Math.Pow((node.Y - e.Y), 2)) < RADIUS * RADIUS)
                        {
                            if (_selectedNode == -1)
                            {
                                _selectedNode = _graph.Nodes.IndexOf(node);
                            }
                            else
                            {
                                if (_graph.Nodes.IndexOf(node) != _selectedNode)
                                {
                                    _graph.Edges.Add(new Edge(_selectedNode, _graph.Nodes.IndexOf(node)));
                                    _selectedNode = -1;
                                }
                            }
                        }
                    }
                    DrawGraph();
                    break;
                case "nodeMoveBtn":
                    if (_selectedNode == -1)
                    {
                        foreach (var node in _graph.Nodes)
                        {
                            if ((Math.Pow((node.X - e.X), 2) + Math.Pow((node.Y - e.Y), 2)) < RADIUS * RADIUS)
                            {
                                _selectedNode = _graph.Nodes.IndexOf(node);
                            }
                        }
                    }
                    else
                    {
                        foreach (var node in _graph.Nodes)
                        {
                            if (((Math.Pow((node.X - e.X), 2) + Math.Pow((node.Y - e.Y), 2)) < 4 * RADIUS * RADIUS) && (_graph.Nodes.IndexOf(node) != _selectedNode))
                            {
                                return;
                            }
                        }
                        _selectedNode = -1;
                    }
                    DrawGraph();
                    break;
                case "edgeDestroyBtn":
                    foreach (var node in _graph.Nodes)
                    {
                        if ((Math.Pow((node.X - e.X), 2) + Math.Pow((node.Y - e.Y), 2)) < RADIUS * RADIUS)
                        {
                            if (_selectedNode == -1)
                            {
                                _selectedNode = _graph.Nodes.IndexOf(node);
                            }
                            else
                            {
                                _graph.Edges = (from edge in _graph.Edges
                                                where !((edge.N1 == _graph.Nodes.IndexOf(node)) &&
                                                (edge.N2 == _selectedNode))
                                                select edge).ToList();

                                _graph.Edges = (from edge in _graph.Edges
                                                where !((edge.N1 == _selectedNode) &&
                                                (edge.N2 == _graph.Nodes.IndexOf(node)))
                                                select edge).ToList();

                                _selectedNode = -1;
                            }
                        }
                    }
                    DrawGraph();
                    break;
                case "clearBtn":
                    _graph.Nodes.Clear();
                    _graph.Edges.Clear();
                    DrawGraph();
                    break;
                default:
                    break;
            }
        }

        private void Btn_Click(object sender, EventArgs e)
        {
            _selectedNode = -1;

            nodeAddBtn.FlatAppearance.BorderSize = 0;
            edgeAddBtn.FlatAppearance.BorderSize = 0;
            nodeDestroyBtn.FlatAppearance.BorderSize = 0;
            nodeMoveBtn.FlatAppearance.BorderSize = 0;
            clearBtn.FlatAppearance.BorderSize = 0;

            (sender as Button).FlatAppearance.BorderSize = 1;
            _action = (sender as Button).Name;
        }

        private void Canvas_MouseMove(object sender, MouseEventArgs e)
        {
            if ((_action == "nodeMoveBtn") && (_selectedNode != -1))
            {
                _graph.Nodes[_selectedNode].X = e.X;
                _graph.Nodes[_selectedNode].Y = e.Y;
                DrawGraph();
            }
        }

        private void ColorBtn_MouseClick(object sender, MouseEventArgs e)
        {
            if (colorDialog.ShowDialog() == DialogResult.OK)
                colorBtn.BackColor = colorDialog.Color;
            _color = colorDialog.Color;
        }

        private void BfsBtn_Click(object sender, EventArgs e)
        {
            bfsHead = 0;
            bfsTail = 1;

            queue[0] = 0;
            _graph.Nodes[0].Slc = true;

            DrawGraph();
            bfsTimer.Enabled = true;
        }

        private void MatrixBtn_Click(object sender, EventArgs e)
        {
            matrixTB.Clear();

            for (int i = 0; i < _graph.Nodes.Count; i++)
            {
                for (int j = 0; j < _graph.Nodes.Count; j++)
                {
                    matrixTB.Text += _matrixArray[j, i].ToString() + " ";
                }
                matrixTB.Text += "\r\n";
            }
        }

        private void BfsTimer_Tick(object sender, EventArgs e)
        {
            if (bfsHead < bfsTail)
            {
                nodeNum = queue[bfsHead];
                for (var i = 0; i <= _graph.Nodes.Count - 1; i++)
                {
                    if ((_matrixArray[i, nodeNum] == 1) && (_graph.Nodes[i].Slc == false))
                    {
                        queue[bfsTail] = i;
                        bfsTail += 1;
                        _graph.Nodes[i].Slc = true;
                    }
                }
                bfsHead += 1;
                DrawGraph();
            }
            else
            {
                if (searchEnd == true)
                {
                    bfsTimer.Enabled = false;
                    searchEnd = false;
                    for (var i = 0; i <= _graph.Nodes.Count - 1; i++)
                    {
                        _graph.Nodes[i].Slc = false;
                    }
                    bfsTimer.Interval = 500;
                    matrixTB.Text = "";
                    DrawGraph();

                }
                else
                {
                    bfsTimer.Interval = 2000;
                    searchEnd = true;
                    isConnected = true;
                    for (var i = 0; i <= _graph.Nodes.Count - 1; i++)
                    {
                        if (_graph.Nodes[i].Slc == false)
                        {
                            isConnected = false;
                        }
                    }
                    if (!isConnected)
                    {
                        matrixTB.Text = "не связный";
                    }
                    else
                    {
                        matrixTB.Text = "связный";
                    }
                }

            }

        }



        public void Dfs(int num)
        {
            queueSize += 1;
            _graph.Nodes[num].Slc = true;
            queue[queueSize] = num;

            for (var i = 0; i <= _graph.Nodes.Count; i++)
            {
                if ((_matrixArray[num, i] != 0) && !(_graph.Nodes[i].Slc))
                {
                    Dfs(i);
                }
            }
        }

        private void DfsBtn_Click(object sender, EventArgs e)
        {
            queueSize = 1;
            Dfs(0);
            v = 1;
            //DrawGraph();
            for (var i = 0; i <= _graph.Nodes.Count - 1; i++)
            {
                _graph.Nodes[i].Slc = false;
            }
            dfsTimer.Enabled = true;
        }

        private void DfsTimer_Tick(object sender, EventArgs e)
        {
            if (v <= queueSize + 1)
            {
                _graph.Nodes[queue[v - 1]].Slc = true;
                DrawGraph();
                v += 1;
            }
            else
            {
                if (searchEnd)
                {
                    dfsTimer.Enabled = false;
                    searchEnd = false;
                    for (var i = 0; i <= _graph.Nodes.Count - 1; i++)
                    {
                        _graph.Nodes[i].Slc = false;
                    }
                    dfsTimer.Interval = 500;
                    matrixTB.Text = "";
                    DrawGraph();
                }
                else
                {
                    dfsTimer.Interval = 1500;
                    searchEnd = true;
                    isConnected = true;
                    for (var i = 0; i <= _graph.Nodes.Count - 1; i++)
                    {
                        if (_graph.Nodes[i].Slc == false)
                        {
                            isConnected = false;
                        }
                    }
                    if (!isConnected)
                    {
                        matrixTB.Text = "не связный";
                    }
                    else
                    {
                        matrixTB.Text = "связный";
                    }
                }
            }
        }
        /* private void DfsBtn_Click(object sender, EventArgs e)
         {
             Dfs(0);
             DrawGraph();
         }

         public void Dfs(int num)
         {
             _graph.Nodes[num].Slc = true;
             for (var i = 0; i <= _graph.Nodes.Count; i++)
             {
                 if ((_matrixArray[v, i] != 0) && (_graph.Nodes[i].Slc == false))
                 {
                     Dfs(i);
                 }
             }

         }*/

        class Node
        {
            public int X { get; set; }
            public int Y { get; set; }
            public Color Cl { get; set; }
            public bool Slc { get; set; }

            public Node(int x, int y, Color cl, bool slc)
            {
                X = x;
                Y = y;
                Cl = cl;
                Slc = slc;
            }
        }
        class Edge
        {
            public int N1 { get; set; }
            public int N2 { get; set; }

            public Edge(int n1, int n2)
            {
                N1 = n1;
                N2 = n2;
            }
        }

        class Graph
        {
            public List<Node> Nodes = new List<Node>();
            public List<Edge> Edges = new List<Edge>();

            public void AddNode(int x, int y, Color cl, bool slc)
            {
                Nodes.Add(new Node(x, y, cl, slc));
            }
            public void AddEdge(int n1, int n2)
            {
                Edges.Add(new Edge(n1, n2));
            }
        }
    }
}
