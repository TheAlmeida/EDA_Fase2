-admins bin file
username|password|name|email

-clients bin file
username|password|name|email|cellphone|age|geolocation|totalkms|nif|totaltrips|balance

-history bin file
user|transport|start|finish|distance|duration|cost

-vehicles bin file
code|type|geolocation|weight|battery|autonomy|costhour|costkm|inUse|totalkms

-graph bin file
nº total loc
locX
nº adj loc of locX
adjacent locs of locX (one adj location plus it's weight per line)
client's username in locX (one client per line)
-1 (indicating end of clients at locX)
vehicle's code and type in locX (one vehicle per line)
-1 (indicating end of vehicles at locX)