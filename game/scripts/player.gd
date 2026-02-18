extends CharacterBody2D
@onready var camera_2d: Camera2D = $Camera2D


const SPEED = 130
const JUMP_VELOCITY = -300.0
var isCurentPlayer = false


func _ready() -> void:
	var current_cam = get_viewport().get_camera_2d()
	if current_cam == null:
		print("Aucune caméra active trouvée. J'active la mienne !")
		isCurentPlayer = true
		camera_2d.make_current()
		camera_2d.enabled = true


func _physics_process(delta: float) -> void:
	
	if not isCurentPlayer:
		return
	
	
	# Add the gravity.
	if not is_on_floor():
		velocity += get_gravity() * delta

	# Handle jump.
	if Input.is_action_just_pressed("ui_accept") and is_on_floor():
		velocity.y = JUMP_VELOCITY

	# Get the input direction and handle the movement/deceleration.
	# As good practice, you should replace UI actions with custom gameplay actions.
	var direction := Input.get_axis("ui_left", "ui_right")
	if direction:
		velocity.x = direction * SPEED
	else:
		velocity.x = move_toward(velocity.x, 0, SPEED)

	move_and_slide()
