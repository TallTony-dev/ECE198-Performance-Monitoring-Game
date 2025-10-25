export async function POST(request) {
  const arrayBuffer = await request.arrayBuffer(); // raw binary data
  const bytes = new Uint8Array(arrayBuffer);

  // process `bytes` here
  console.log('Received', bytes.length, 'bytes');

  return new Response(JSON.stringify({ received: bytes.length }), {
    status: 200,
    headers: { 'Content-Type': 'application/json' },
  });
}