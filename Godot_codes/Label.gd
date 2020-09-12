extends Label

# Declare member variables here. Examples:
var a = 0
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	a += delta
	text = str(a) # 'text' is a built-in label property.

