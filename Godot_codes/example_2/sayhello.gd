extends Panel

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	get_node("Button").connect("button_down", self, "_on_Button_button_down")
	get_node("Button").connect("button_up", self, "_on_Button_button_up")
	get_tree().call_group("enemies", "player_was_discovered")
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass



func _on_Button_button_down():
	get_node("Label").text = "HELLO!"
	pass # Replace with function body.


func _on_Button_button_up():
	get_node("Label").text = "BYE!"
	pass # Replace with function body.
