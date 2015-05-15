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

/*This is a skeleton data parser write by Tut Tangtragulcharoen*/

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

    /* The commented out code above is another JSON parser. It cannot be read by the Unreal Engine. It parsed data as a collection of bodies, and within those bodies , array of joints. 
 * This is no longer relevent to the LOVR project. Howver, it might come in hand later for some future functionality*/

    public static class SkeletonSerializer
    {
        //public static local_mem[] IDs;

        public class output
        {
            public Avatar avatar;
            public List<JSONJoint> create;
            public List<JSONJoint> move;
            public List<JSONJoint> Destroy;
        }

        public class Avatar
        {
            public Boolean changed;
            public Boolean change_rotation;
            public double[] position;
            public double rotation;
        }

        public class JSONJoint
        {   
            public string name;
            public double[] position = new double[2];
            public double radius;
        }

        /*public class local_mem
        {
            string id;
            Joint[] prev_frame;

             public local_mem(string new_id)
             {
                id = new_id;
                prev_frame = new Joint[interestedJointTypes.Length];
                for (int i = 0; i < interestedJointTypes.Length; i++)
                {
                    prev_frame[i].JointType = interestedJointTypes[i];
                }
             }

            public void hard_copy(Body new_frame)
            {
                foreach (Joint joint in new_frame.Joints.Values)
                {
                    for (int i = 0; i < prev_frame.Length; i++)
                    {
                        if (prev_frame[i].JointType == joint.JointType)
                        {
                            prev_frame[i].Position.X = joint.Position.X;
                            prev_frame[i].Position.Y = joint.Position.Y;
                            prev_frame[i].Position.Z = joint.Position.Z;
                        }
                    }
                }
            }
        }*/

        /*The section of code above is for storing the joint position in the previous frames in order to do filtering. But from the tests, filtering is not required and might cause a lot of lag*/

        public static void Serialize(Body skeleton)
        {
            var obj = new output
            {
                avatar = create_avatar(skeleton), //avatar mode activated
                //avatar = default_avatar(), //avatar mode deactivated
                /*comment out either line above to activate and deactivate the avatar mode*/
                create = new List<JSONJoint>(),
                move = move_parser(skeleton),
                Destroy = new List<JSONJoint>()
            };
            var json = new JavaScriptSerializer().Serialize(obj);
            string curFile = @"C:/Users/amshah4/Documents/GitHub/VIVE/update.json"; //change to where every VIVE reads from
            if (json.Length > 30 && !File.Exists(curFile))
            {
                    System.IO.File.WriteAllText(curFile, json);
            }
         }



        public static Avatar create_avatar(Body skeleton)
        {
            double theta = -90.0;
            Avatar a = new Avatar();
            a.changed = true;
            a.change_rotation = false;
            a.position = new double[3];
            foreach (Joint joint in skeleton.Joints.Values)
            {
                if (joint.JointType.ToString().ToLower() == "head")
                {
                    //a.position[0] = (joint.Position.Z * 1000) * Math.Cos(theta) + (joint.Position.X * -1000) * Math.Sin(theta);
                    //a.position[1] = (joint.Position.X * -1000) * Math.Sin(theta) - (joint.Position.Z * 1000) * Math.Cos(theta);
                    /*The two lines above are the new xy joint position, after rotation using the rotation matrix*/
                    a.position[0] = -1 * (joint.Position.Z * 1000); //JACK: I negated these two values to match the negated values of the head (read below)
                    a.position[1] = -1 * (joint.Position.X * -1000);
                    a.position[2] = (joint.Position.Y * 1000)-250;
                }
            }
            a.rotation = 0.0;
            return a;
        }

        public static List<JSONJoint> move_parser(Body skeleton)
        {
            List<JSONJoint> temp_jointList = new List<JSONJoint>();
            double theta = -90.0;
            foreach (Joint joint in skeleton.Joints.Values)
            {
                JSONJoint temp_joint = new JSONJoint();
                foreach (var jointType in interestedJointTypes)
                {
                    if(joint.JointType == jointType)
                    {
                        temp_joint.name = joint.JointType.ToString().ToLower()+"1";
                        //temp_joint.name = temp_joint.name + ID;
                        //add ID to the joints name
                        temp_joint.position = new double[3];
                        //temp_joint.position[0] = (joint.Position.Z * 1000)*Math.Cos(theta)+(joint.Position.X * -1000)*Math.Sin(theta);
                       // temp_joint.position[1] = (joint.Position.X * -1000)*Math.Sin(theta)-(joint.Position.Z * 1000)*Math.Cos(theta);
                        /*The two lines above are the new xy joint position, after rotation using the rotation matrix*/
                        temp_joint.position[0] = -1 * (joint.Position.Z * 1000); //JACK: I negated these two values ( Z and X ) because they rendered backwards in Kinect, so now they are rendered frontwards in Unreal.
                        temp_joint.position[1] = -1 * (joint.Position.X * -1000);
                        temp_joint.position[2] = joint.Position.Y * 1000;
                        temp_joint.radius = 5.0;
                        if (temp_joint.name == "neck1" || temp_joint.name == "elbowleft1" || temp_joint.name == "elbowright1")
                        {
                            temp_joint.radius = 2.5;
                        }
                        temp_jointList.Add(temp_joint);
                    }
                }
            }
            return temp_jointList;
        }

        public class output1
        {
            public Avatar avatar;
            public List<JSONJoint> create;
            public List<JSONJoint> move;
            public List<JSONJoint> Destroy;
        }

        public static void Serialize_default()
        {
            var obj1 = new output1
            {
                avatar = default_avatar(),
                create = default_create(),
                move = new List<JSONJoint>(),
                Destroy = new List<JSONJoint>()
            };
            var json = new JavaScriptSerializer().Serialize(obj1);
            System.IO.File.WriteAllText(@"C:/Users/amshah4/Documents/GitHub/VIVE/update.json", json); //change to where every VIVE reads from
        }

        public static Avatar default_avatar()
        {
            Avatar a = new Avatar();
            a.changed = false;
            a.change_rotation = true;
            a.position = new double[3];
            a.position[0] = 0.0;
            a.position[1] = 0.0;
            a.position[2] = 0.0;
            a.rotation = 45.0;
        
            return a;
        }

        public static List<JSONJoint> default_create()
        {
            List<JSONJoint> temp_jointList = new List<JSONJoint>();
       
                foreach (var jointType in interestedJointTypes)
                {
                    JSONJoint temp_joint = new JSONJoint();
                    temp_joint.name = jointType.ToString().ToLower()+"1";
                    temp_joint.position = new double[3];
                    temp_joint.position[0] = 1;
                    temp_joint.position[1] = 1;
                    temp_joint.position[2] = 1;
                    temp_joint.radius = 5.0;/////////////////////////////ANKOOR CHANGED FROM 1000.0
                    if (temp_joint.name == "neck1" || temp_joint.name == "elbowleft1" || temp_joint.name == "elbowright1")
                    {
                        temp_joint.radius = 2.5;
                    }
                    temp_jointList.Add(temp_joint);
                }
            return temp_jointList;
        }

        static JointType[] interestedJointTypes = 
    {
      //JointType.Head,
      /*The head should be commented out when Avatar mode is activated. Otherwise nothing can be seen except for the inside of the head*/
      JointType.Neck,
      JointType.ShoulderLeft,
      JointType.ShoulderRight,
      JointType.HandLeft,
      JointType.HandRight,
      JointType.ElbowLeft,
      JointType.ElbowRight,
      JointType.HipLeft,
      JointType.HipRight,
      JointType.KneeLeft,
      JointType.KneeRight,
      JointType.AnkleLeft,
      JointType.AnkleRight,
      JointType.FootLeft,
      JointType.FootRight,
      JointType.SpineBase,
      JointType.SpineMid,
      JointType.SpineShoulder
    };
    }
}