
newCG("Triggered")

newSubCG("Triggered", "RunAway")
newCom("RunAway", "MoveToEaseIO",  "ivysaur", 0, 0, 0, 6, 0.3, 0.3, 0, "knight2", "")
newCom("RunAway", "OrientTo",  "ivysaur", 0, 0, 0, 1, 0, 0, 0, "knight2", "")

newSubCG("Triggered", "NextRunPoint")
newCom("NextRunPoint", "MoveToEaseIO",  "ivysaur", 0, 0, 0, 6, 0.3, 0.3, 0, "AxePainted", "")
newCom("NextRunPoint", "OrientTo",  "ivysaur", 0, 0, 0, 1, 0, 0, 0, "AxePainted", "")

newSubCG("Triggered", "NextRunPoint1")
newCom("NextRunPoint1", "MoveToEaseIO",  "ivysaur", 0, 0, 0, 3, 0.3, 0.3, 0, "cat", "")
newCom("NextRunPoint1", "OrientTo",  "ivysaur", 0, 0, 0, 1, 0, 0, 0, "cat", "")

newSubCG("Triggered", "NextRunPoint2")
newCom("NextRunPoint2", "MoveToEaseIO",  "ivysaur", 0, 0, 0, 3, 0.3, 0.3, 0, "", "")
newCom("NextRunPoint2", "OrientTo",  "ivysaur", 0, 0, 0, 1, 0, 0, 0, "AxePainted", "")

--camera will follow Ivysaur for 15 sec IN PARALLEL
newCom("Triggered", "Follow",  "camera", 450, 400, 400, 25, 400, 500, 800, "ivysaur", "")
