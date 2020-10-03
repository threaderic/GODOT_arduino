extends Node

# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Declare arduino connection to Godot ;)
var aaa = Arduino_f
var arduino_string
var chrono=0
var received_String="hello"
#var temperature
var String_splited = [0,0,0,0,0,0,0]
var quat_MPU = Quat(0.0,0.0,0.0,0.0)
var card_3D

# Called when the node enters the scene tree for the first time.
func _ready():
	aaa = Arduino_f.new()
	aaa.Thread_generate_com()
	
	#3D simple modelisation of arduino + MPU9250
	card_3D = get_node("ViewportContainer/RigidBody2")

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_Timer_timeout():
	# Note: the `$` operator is a shorthand for `get_node()`,
	# so `$Sprite` is equivalent to `get_node("Sprite")`.
	$Sprite.visible = !$Sprite.visible


func _on_Timer2_timeout():
	chrono +=0.1
	
	if chrono > 5:
		received_String = aaa.get_string_com()
		$Label.text = received_String
		received_String = received_String.lstrip("<")
		String_splited = received_String.split_floats(",")
		quat_MPU.w = -String_splited[3]
		quat_MPU.x = String_splited[2]
		quat_MPU.y = String_splited[0]
		quat_MPU.z = String_splited[1]
		arduino_string = String_splited[4]
		get_node("ViewportContainer/RigidBody2").transform = Transform(quat_MPU)

		
func _on_HScrollBar_scrolling():
	$Label2.text = String($HScrollBar.value)
	aaa.set_string_com("<Hello   ," + String(int($HScrollBar.value)).pad_zeros(3) + ",8.9>")

