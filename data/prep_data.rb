# Prep the RSBN data for import into the plugin.
# Does a few things: converts the data in file into a more easily parseable format,
# fetches elevation data from NASA
# 
# Elevation magic URL:
# http://gisdata.usgs.gov/xmlwebservices2/elevation_service.asmx/getElevation?X_Value=57.8&Y_Value=28.5&Elevation_Units=METERS&Source_Layer=-1&Elevation_Only=true

ELEV_URI = "http://gisdata.usgs.gov/xmlwebservices2/elevation_service.asmx/getElevation?X_Value=%d&Y_Value=%d&Elevation_Units=METERS&Source_Layer=-1&Elevation_Only=true"

require "open-uri"
File.open("cis_raw.txt") do | f |
  File.open("cis.dat", "w") do | out |
    7.times { f.readline }
  
    until f.eof? do
      # 37k RSBN PSKOV
      channel, name = f.readline.scan(/(\d\d)k RSBN (.+)/).flatten.map{|e| e.strip}
      callsign = f.readline.strip
      vor_freq = f.readline.strip
 
      lat, lon = [f.readline, f.readline].map do | el |
        deg, min = el.split
        deg.to_f + (min.to_f/60) 
      end
    
      # Ask NASA about the elevation
      req_url = ELEV_URI % [lat, lon]
      elev = open(req_url).read.scan(/>(.+)</).flatten[0].to_f
      
      out.puts [channel, name, callsign, vor_freq, lat, lon, elev].join('|')
    end
  end
end