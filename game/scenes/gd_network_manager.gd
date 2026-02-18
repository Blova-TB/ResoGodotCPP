extends GDNetworkManager

@export var port = 5000

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	if (bind_port(port)):
		print("binded to the port !")
	var data = "Helo World".to_utf8_buffer()
	send_packet("127.0.0.1", port, data)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	poll()


func _on_packet_received(sender_ip: String, sender_port: int, data: PackedByteArray) -> void:
	print(sender_ip , sender_port , data.get_string_from_ascii())
