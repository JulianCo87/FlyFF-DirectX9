dofile( ".\\LuaFunc\\BotBehaviorFunc.lua" )

addUserBotType("test", "MI_FEMALE", "JOB_RINGMASTER_HERO", 121)
-- {

    spawn("WI_WORLD_MADRIGAL", 6959, 100, 3309)

    addItem("II_GEN_FOO_INS_BISCUIT", 1)
    addItem("II_SYS_BLI_TWO_TOWNBLINKWING",5)
    addItem("II_GEN_FOO_INS_LOLLIPOP",5)

    addEquipment("II_WEA_CHE_BEGIN")
	addEquipment("II_ARM_F_RIN_HELMET08")
    addEquipment("II_ARM_F_RIN_SUIT08")
    addEquipment("II_ARM_F_RIN_GAUNTLET08")
    addEquipment("II_ARM_F_RIN_BOOTS08")


    addOnlineTime("23:00", MIN(45))
    --{
        addBehaviour("idle", -1)
    --}
    addOnlineTime("04:20", HRS(random(2,3)))
    --{
        addBehaviour("idle", -1)
    --} 
    addOnlineTime("22:02", MIN(random(10, 30)))
    --{
        addBehaviour("idle", -1)
    --} 
    addOnlineTime("4:10", MIN(random(10, 30)))
    --{
        addBehaviour("idle", -1)
    --}
    addOnlineTime("18:45", MIN(random(10, 30)))
    --{
        addBehaviour("idle", -1)
    --}	
-- }
