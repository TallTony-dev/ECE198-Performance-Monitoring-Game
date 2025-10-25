'use client';

import { useState } from 'react';

export default function Home() {
  const [response, setResponse] = useState('');

  async function sendTestData() {
    // create fake binary data for test
    const bytes = new Uint8Array([1, 2, 3, 4]);
    const res = await fetch('/api/apitest', {
      method: 'POST',
      body: bytes,
    });
    const data = await res.json();
    setResponse(JSON.stringify(data));
  }

  return (
    <main style={{ padding: 20 }}>
      <button onClick={sendTestData}>Send Test Binary</button>
      <p>Response: {response}</p>
    </main>
  );
}