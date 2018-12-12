--Create New group 
newCG("Main")
--Create New Command SubGroup
newSubCG("Main", "PickUp_pockeball")
--Create New commands and add it to existing command group to run them parallel
-- (Group Name, CommandName, objName, x, y, z, time, EaseIn from 0 to 1, EaseOut, Target Obj, ScriptName)
newCom("PickUp_pockeball", "MoveToEaseIO",  "knight", 0, 0, 0, 8, 0.3, 0.3, 0, "pokeball", "")
newCom("PickUp_pockeball", "OrientTo",  "knight", 0, 0, 0, 7, 0, 0, 0, "pokeball", "")
--Command group that will run After previous CG is finished
newSubCG("Main", "PrepareToThrow")
newCom("PrepareToThrow", "MoveToEaseIO",  "knight", 0, 0, 0, 4, 0.4, 0.4, 0, "teapot", "")
--example of "follow" command
--where  (..., ..., ... IdealRelPos, FollowTime(0 = forever), MinDist, MaxSpeedDis, MaxSpeed, target)
newCom("PrepareToThrow", "Follow",  "pokeball", 100, 100, 100, 4, 50, 200, 600, "knight", "")
--Aiming to throw pokeball
newSubCG("Main", "Aim")
newCom("Aim", "OrientTo",  "knight", 0, 0, 0, 3, 0, 0, 0, "ivysaur", "")
--Throw!
newSubCG("Main", "ballisticThrow")
--example of follow Bezier curve command
--where  (..., ..., obj to move, x,y,z -control point, time, x,y,z destination OR destination object)
newCom("ballisticThrow", "FollowCurve", "pokeball", 300, 1000, 0, 4, 0, 0, 0, "ivysaur", "")

--this command will run in parallel on Main Group
--pokeball trigged ivysaur
--example of trigger command
--where (..., ..., obj that we checking, x,y,z -trigger point (if no target object), radius, object-location trigger, script that will run)
newCom("Main", "Trigger",  "pokeball", 0, 0, 0, 50, 0, 0, 0, "ivysaur", "trig.lua")
