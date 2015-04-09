using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.IO;
using Microsoft.Kinect;
using System;
using System.Web.Script.Serialization;
using System.Diagnostics;
using System.Threading;

namespace ConsoleApplication8
{
    /*
    /// <summary>
    /// Serializes a Kinect skeleton to JSON fromat.
    /// </summary>
    public static class SkeletonSerializer
    {
        [DataContract]
        class JSONSkeletonCollection
        {
            [DataMember(Name = "skeletons")]
            public List<JSONSkeleton> Skeletons { get; set; }
        }

        [DataContract]
        class JSONSkeleton
        {
            [DataMember(Name = "id")]
            public string ID { get; set; }

            [DataMember(Name = "joints")]
            public List<JSONJoint> Joints { get; set; }
        }

        [DataContract]
        class JSONJoint
        {
            [DataMember(Name = "name")]
            public string Name { get; set; }

            [DataMember(Name = "x")]
            public double X { get; set; }

            [DataMember(Name = "y")]
            public double Y { get; set; }

            [DataMember(Name = "z")]
            public double Z { get; set; }

            [DataContract]
            class Output
            {
                [DataMember(Name = "avatar")]
                public string avt { get; set; }

                [DataMember(Name = "create")]
                public string empt1 { get; set; }

                [DataMember(Name = "move")]
                public List<JSONJoint> Joints {get; set;}

                [DataMember(Name = "Destroy")]
                public string empt2 { get; set; }
            }


            class Avatar
            {
                [DataMember(Name = "changed")]
                public Boolean change = false;

                [DataMember(Name = "position")]
                public List<int> pos = new List<int>();

                [DataMember(Name = "rotation")]
                public int rot = 0;

            }

            class JSONJoint
            {
                [DataMember(Name = "name")]
                public string Name { get; set; }

                
                [DataMember(Name = "position")]
                public List<int> xyz = new List<int>();

                [DataMember(Name = "rotation")]
                public double rad = 1.0;
            }
        }


        }

        public static string Serialize(this List<Body> skeletons)
        {
            JSONSkeletonCollection jsonSkeletons = new JSONSkeletonCollection { Skeletons = new List<JSONSkeleton>() };

            foreach (var skeleton in skeletons)
            {
                JSONSkeleton jsonSkeleton = new JSONSkeleton
                {
                    ID = skeleton.TrackingId.ToString(),
                    Joints = new List<JSONJoint>()
                };

                foreach (Joint joint in skeleton.Joints.Values)
                {

                    jsonSkeleton.Joints.Add(new JSONJoint
                    {
                        Name = joint.JointType.ToString().ToLower(),
                        X = joint.Position.X,
                        Y = joint.Position.Y,
                        Z = joint.Position.Z
                    });
                }

                jsonSkeletons.Skeletons.Add(jsonSkeleton);
            }

            return Serialize(jsonSkeletons);
        }

        public static string Serialize(this List<Body> skeletons)
        {
            Output out1 = new Output();
            out1.avt = "";
            
        }

        // Resource: http://pietschsoft.com/post/2008/02/NET-35-JSON-Serialization-using-the-DataContractJsonSerializer.aspx.
        private static string Serialize(object obj)
        {
            DataContractJsonSerializer serializer = new DataContractJsonSerializer(obj.GetType());
            MemoryStream ms = new MemoryStream();
            serializer.WriteObject(ms, obj);
            string retVal = Encoding.Default.GetString(ms.ToArray());
            ms.Dispose();

            return retVal;
        }
    }*/



    public static class SkeletonSerializer
    {

        public class output
        {
            public Avatar avatar;
            public List<JSONJoint> create;
            public List<JSONJoint> move;
            public string Destroy;
        }

        public class Avatar
        {
            public Boolean changed;
            public double[] position;
            public int rotation;
        }

        public class JSONJoint
        {   
            public string name;
            public double[] position = new double[2];
            public double radius;
        }


        public static void Serialize(Body skeleton)
        {
            var obj = new output
            {
                //avatar = temp(skeleton),
                avatar = temp3(),
                create = new List<JSONJoint>(),
                move = temp2(skeleton),
                Destroy = "[]"
            };
            var json = new JavaScriptSerializer().Serialize(obj);
            //string curFile2 = @"C:/Users/amshah4/Desktop/Unreal_stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json";
            string curFile = @"C:/Users/amshah4/Documents/GitHub/VIVE/update.json";
            //string curFile = @"C:/Users/amshah4/Documents/GitHub/VIVE/update.json";
            if (json.Length > 30 && !File.Exists(curFile))
            {
                //try
                {

                    System.IO.File.WriteAllText(curFile, json);
                    //System.IO.File.WriteAllText(curFile2, json);
                    //System.IO.File.WriteAllText(@"C:/Users/amshah4/Desktop/output.json", json);
                }
                //catch (IOException ex)
                //{
                    //An I/O error occured when opening the file
                //}
            }
         }



        public static Avatar temp(Body skeleton)
        {
            Avatar a = new Avatar();
            a.changed = true;
            a.position = new double[3];
            foreach (Joint joint in skeleton.Joints.Values)
            {
                if (joint.JointType.ToString().ToLower() == "head")
                {
                    a.position[0] = joint.Position.Z * 1000;
                    a.position[1] = joint.Position.X * 1000;
                    a.position[2] = (joint.Position.Y * 1000)-600;
                }
            }
            a.rotation = 0;
            return a;
        }

        public static List<JSONJoint> temp2(Body skeleton)
        {
            List<JSONJoint> blah = new List<JSONJoint>();
            //string ID = skeleton.TrackingId.ToString();
            foreach (Joint joint in skeleton.Joints.Values)
            {
                JSONJoint temp1 = new JSONJoint();
                foreach (var jointType in interestedJointTypes)
                {
                    if(joint.JointType == jointType)
                    {
                        temp1.name = joint.JointType.ToString().ToLower()+"1";
                        //temp1.name = temp1.name + ID;
                        temp1.position = new double[3];
                        temp1.position[0] = joint.Position.Z * 1000;
                        temp1.position[1] = joint.Position.X * 1000;
                        temp1.position[2] = joint.Position.Y * 1000;
                        temp1.radius = 1000.0;
                        blah.Add(temp1);
                    }
                }
            }
            return blah;
        }

        public class output1
        {
            public Avatar avatar;
            public List<JSONJoint> create;
            public string move;
            public string Destroy;
        }

        public static void Serialize1()
        {
            var obj1 = new output1
            {
                avatar = temp3(),
                create = temp4(),
                move = "",
                Destroy = ""
            };
            var json = new JavaScriptSerializer().Serialize(obj1);
            //System.IO.File.WriteAllText(@"C:/Users/amshah4/Desktop/Unreal_stuff/ViveView/Saved/StagedBuilds/WindowsNoEditor/ViveView/Binaries/Win64/ActorList.json", json);
            System.IO.File.WriteAllText(@"C:/Users/amshah4/Documents/GitHub/VIVE/update.json", json);
        }

        public static Avatar temp3()
        {
            Avatar a = new Avatar();
            a.changed = false;
            a.position = new double[3];
            a.position[0] = 0.0;
            a.position[1] = 0.0;
            a.position[2] = 0.0;
            a.rotation = 0;
            return a;
        }

        public static List<JSONJoint> temp4()
        {
            List<JSONJoint> blah = new List<JSONJoint>();
       
                foreach (var jointType in interestedJointTypes)
                {
                    //Console.Write(jointType);
                    JSONJoint temp1 = new JSONJoint();
                    temp1.name = jointType.ToString().ToLower()+"1";
                    //Console.Write(temp1.name);
                    temp1.position = new double[3];
                    temp1.position[0] = 1;
                    temp1.position[1] = 1;
                    temp1.position[2] = 1;
                    temp1.radius = 5.0;/////////////////////////////ANKOOR CHANGED FROM 1000.0
                    blah.Add(temp1);
                    //Thread.Sleep(100);
                }
            return blah;
        }

        static JointType[] interestedJointTypes = 
    {
      JointType.Head,
      /*JointType.Neck,
      JointType.ShoulderLeft,
      JointType.ShoulderRight,*/
      JointType.HandLeft,
      JointType.HandRight/*,
      JointType.ElbowLeft,
      JointType.ElbowRight,
      JointType.HipLeft,
      JointType.HipRight,
      JointType.KneeLeft,
      JointType.KneeRight,
      JointType.AnkleLeft,
      JointType.AnkleRight,
      JointType.FootLeft,
      JointType.FootRight*/
    };
    }
}