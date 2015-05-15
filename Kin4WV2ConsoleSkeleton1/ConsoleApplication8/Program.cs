/*Ackhnowledgement to Mike Taulty who help me greatly with this code*/
using Microsoft.Kinect;
using System;

namespace ConsoleApplication8
{
  class Program
  {
    static void Main(string[] args)
    {
        SkeletonSerializer.Serialize_default();
      Console.SetWindowSize(Constants.ConsoleWidth, Constants.ConsoleHeight);
      Console.Title = "Kinect Skeleton Console";

      KinectControl c = new KinectControl(() => new ConsoleBodyDrawer());

      c.GetSensor();
      c.OpenReader();

      while (true)
      {
        var k = Console.ReadKey(true).Key;

        if (k == ConsoleKey.X)
        {
          c.CloseReader();
          c.ReleaseSensor();
          break;
        }
      }
    }
  }
}