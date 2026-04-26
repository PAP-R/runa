use bevy::{ecs::event::Trigger, picking::pointer::PointerInteraction, prelude::*};

#[derive(Component)]
pub struct Stroke {
    pub points: Vec<Vec2>,
    pub color: Color,
    pub thickness: f32,
}

#[derive(Component)]
pub struct ActiveStroke;

#[derive(Resource, Default)]
pub struct PenSettings {
    pub current_color: Color,
    pub current_thickness: f32,
}

#[unsafe(no_mangle)]
pub fn capture_input(
    window_q: Query<&Window>,
    buttons: Res<ButtonInput<MouseButton>>,
    mut commands: Commands,
    mut current_stroke_q: Query<(Entity, &mut Stroke), With<ActiveStroke>>,
    settings: Res<PenSettings>,
) {
    if let Ok(window) = window_q.single()
        && let Some(position) = window.cursor_position()
    {
        let point = Vec2::new(
            position.x - (window.width() / 2.0),
            (window.height() / 2.0) - position.y,
        );

        if buttons.just_pressed(MouseButton::Left) {
            commands.spawn((
                Stroke {
                    points: vec![point],
                    color: settings.current_color,
                    thickness: settings.current_thickness,
                },
                ActiveStroke,
            ));
        } else if buttons.pressed(MouseButton::Left) {
            if let Ok((_, mut stroke)) = current_stroke_q.single_mut() {
                stroke.points.push(point);
            }
        } else if buttons.just_released(MouseButton::Left) {
            for (entity, _) in &current_stroke_q {
                commands.entity(entity).remove::<ActiveStroke>();
            }
        }
    }
}

#[unsafe(no_mangle)]
pub fn render_strokes(mut gizmos: Gizmos, stroke_q: Query<&Stroke>) {
    for stroke in &stroke_q {
        if stroke.points.len() > 1 {
            gizmos.linestrip_2d(stroke.points.iter().cloned(), stroke.color);
        }
    }
}
