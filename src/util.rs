use wgpu::util::DeviceExt;

pub struct DynamicBuffer {
    pub buffer: wgpu::Buffer,
    size: u64,
    reserved: u64,
    usage: wgpu::BufferUsages,
}

impl DynamicBuffer {
    pub fn new(
        device: &wgpu::Device,
        label: Option<&str>,
        size: u64,
        usage: wgpu::BufferUsages,
    ) -> Self {
        let buffer = device.create_buffer(&wgpu::BufferDescriptor {
            label,
            size,
            usage,
            mapped_at_creation: false,
        });

        Self {
            buffer,
            size,
            reserved: size,
            usage,
        }
    }
}
