import { useState, useEffect, useRef } from "react";
import { BrowserMultiFormatReader, Result } from "@zxing/browser";
import { Button } from "@/components/ui/button";
import { Card, CardContent } from "@/components/ui/card";
import { Loader2, Camera } from "lucide-react";

export default function App() {
  const [item, setItem] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);
  const [scanning, setScanning] = useState(false);
  const [error, setError] = useState<string | null>(null);
  const videoRef = useRef<HTMLVideoElement>(null);
  const codeReader = useRef(new BrowserMultiFormatReader());

  const handleScanClick = () => {
    setScanning(true);
    setItem(null);
    setError(null);
  };

  useEffect(() => {
    if (scanning && videoRef.current) {
      codeReader.current
        .decodeOnceFromVideoDevice(undefined, videoRef.current)
        .then((result: Result) => {
          setLoading(true);
          setScanning(false);
          // Simulate database lookup delay
          setTimeout(() => {
            setItem(`Item with code ${result.getText()} found at Walmart, Amazon, and Target.`);
            setLoading(false);
          }, 1500);
        })
        .catch(err => {
          setError("Failed to read barcode. Please try again.");
          setScanning(false);
          console.error(err);
        });
    }

    return () => {
      codeReader.current.reset();
    };
  }, [scanning]);

  return (
    <div className="min-h-screen flex flex-col items-center justify-center bg-gradient-to-b from-blue-100 to-white text-gray-800 p-6">
      <Card className="p-6 rounded-2xl shadow-xl w-full max-w-md text-center">
        <CardContent>
          <h1 className="text-3xl font-bold mb-6">Item Scanner Finder</h1>

          {!scanning && (
            <Button
              onClick={handleScanClick}
              disabled={loading}
              className="px-6 py-2 text-lg rounded-xl"
            >
              {loading ? (
                <>
                  <Loader2 className="mr-2 h-5 w-5 animate-spin" />
                  Scanning...
                </>
              ) : (
                <>
                  <Camera className="mr-2 h-5 w-5" /> Start Scan
                </>
              )}
            </Button>
          )}

          {scanning && (
            <div className="mt-4">
              <video ref={videoRef} className="rounded-xl w-full" autoPlay />
              <p className="mt-2 text-sm text-blue-500">Point your camera at a barcode...</p>
            </div>
          )}

          {loading && <p className="mt-4 text-blue-600 font-medium">Searching database...</p>}
          {item && !loading && <p className="mt-4 text-gray-700 text-lg">{item}</p>}
          {error && <p className="mt-4 text-red-500">{error}</p>}
        </CardContent>
      </Card>
    </div>
  );
}
import React from "react";
import ReactDOM from "react-dom/client";
import App from "./App";
import "./index.css";
import * as serviceWorkerRegistration from "./serviceWorkerRegistration";

ReactDOM.createRoot(document.getElementById("root") as HTMLElement).render(
  <React.StrictMode>
    <App />
  </React.StrictMode>
);

serviceWorkerRegistration.register();
