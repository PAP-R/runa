use bevy::prelude::*;
use hot_lib_reloader::hot_module;
use lib::{ActiveStroke, PenSettings, Stroke};

#[hot_module(dylib = "lib")]
mod hot_lib {
    use bevy::prelude::*;
    use lib::{ActiveStroke, PenSettings, Stroke};
    hot_functions_from_file!("lib/src/lib.rs");
}

fn main() {
    App::new()
        .add_plugins(DefaultPlugins)
        .insert_resource(PenSettings {
            current_color: Color::srgb(0.5, 0.0, 1.0),
            current_thickness: 1.0,
        })
        .add_systems(Startup, |mut commands: Commands| {
            commands.spawn(Camera2d::default());
        })
        .add_systems(Update, (hot_lib::capture_input, hot_lib::render_strokes))
        .run();
}
