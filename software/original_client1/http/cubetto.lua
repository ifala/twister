

local function sendCommand(connection, command, param)

	if command == 'servo' then
		--print 'Moving servo'
		uart.write(0,"#1",param)
	elseif command == 'led' then
		--print 'Blinking led'
		uart.write(0,"#0",param)
	else
		print 'Unknown command!'
	end

   -- Send back JSON response.
   connection:send("HTTP/1.0 200 OK\r\nContent-Type: application/json\r\nCache-Control: private, no-store\r\n\r\n")
   connection:send('{"error":0, "message":"OK"}')

end

return function (connection, args)
   --print('Received command!', args.id)
   --print('Params', args.param)
   sendCommand(connection, args.id, args.param)
   -- if     args.door == "1" then pushTheButton(connection, 3)   -- GPIO0
   -- elseif args.door == "2" then pushTheButton(connection, 4)   -- GPIO2
   -- else
   --    connection:send("HTTP/1.0 400 OK\r\nContent-Type: application/json\r\nCache-Control: private, no-store\r\n\r\n")
   --    connection:send('{"error":-1, "message":"Bad door"}')
   -- end
end
