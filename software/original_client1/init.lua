-- Begin WiFi configuration

local wifiConfig = {}

-- wifi.STATION         -- station: join a WiFi network
-- wifi.AP              -- access point: create a WiFi network
-- wifi.wifi.STATIONAP  -- both station and access point
wifiConfig.mode = wifi.STATIONAP  -- both station and access point

wifiConfig.accessPointConfig = {}
wifiConfig.accessPointConfig.ssid = ""   -- Name of the SSID you want to create
wifiConfig.accessPointConfig.pwd = ""    -- WiFi password - at least 8 characters

wifiConfig.stationPointConfig = {}
wifiConfig.stationPointConfig.ssid = "3dita.falaschi.com"    -- Name of the WiFi network you want to join
wifiConfig.stationPointConfig.pwd =  "troa0000"            -- Password for the WiFi network

-- Tell the chip to connect to the access point

wifi.setmode(wifiConfig.mode)

cfg =
  {
    ip="192.168.43.101",
    netmask="255.255.255.0",
    gateway="192.168.43.1"
  }
  wifi.sta.setip(cfg)


print('set (mode='..wifi.getmode()..')')
print('MAC: ',wifi.sta.getmac())
print('chip: ',node.chipid())
print('heap: ',node.heap())


-- End WiFi configuration

-- Compile server code and remove original .lua files.
-- This only happens the first time afer the .lua files are uploaded.

local compileAndRemoveIfNeeded = function(f)
   if file.open(f) then
      file.close()
      print('Compiling:', f)
      node.compile(f)
      file.remove(f)
      collectgarbage()
   end
end

local serverFiles = {'httpserver.lua', 'httpserver-request.lua', 'httpserver-static.lua', 'httpserver-header.lua', 'httpserver-error.lua'}
for i, f in ipairs(serverFiles) do compileAndRemoveIfNeeded(f) end

compileAndRemoveIfNeeded = nil
serverFiles = nil
collectgarbage()

dofile("httpserver.lc")(80)
-- Connect to the WiFi access point.
-- Once the device is connected, you may start the HTTP server.

