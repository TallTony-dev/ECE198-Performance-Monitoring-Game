export async function POST(request) {
  const arrayBuffer = await request.arrayBuffer(); // raw binary data
  const bytes = new Uint8Array(arrayBuffer);

  // process `bytes` here
  console.log('Received', bytes.length, 'bytes');

  // return the length and the actual bytes (as array of numbers)
  return new Response(
    JSON.stringify({ received: bytes.length, bytes: Array.from(bytes) }),
    {
      status: 200,
      headers: { 'Content-Type': 'application/json' },
    }
  );
}