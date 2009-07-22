# Prep the RSBN data for import into the plugin.
# Does a few things: converts the data in file into a more easily parseable format,
# fetches elevation data from NASA
# 
# Elevation magic URL:
# http://gisdata.usgs.gov/xmlwebservices2/elevation_service.asmx/getElevation?X_Value=57.8&Y_Value=28.5&Elevation_Units=METERS&Source_Layer=-1&Elevation_Only=true

ELEV_URI = "http://gisdata.usgs.gov/xmlwebservices2/elevation_service.asmx/getElevation?X_Value=%.3f&Y_Value=%.3f&Elevation_Units=METERS&Source_Layer=-1&Elevation_Only=true"

require "open-uri"
%w( ussr cis).each do | dataset |
  File.open("#{dataset}_raw.txt") do | f |
    File.open("#{dataset}.dat", "w") do | out |
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
        req_url = ELEV_URI % [lat.to_i, lon.to_i]
        elev = open(req_url).read
      
        puts elev
        STDOUT.flush
      
        elev = elev.scan(/>(.+)</).flatten[0].to_f
      
        # If the elevation is minus IDUNNOHOWMUCH meters assume that it's
        # a terrible, terrible, terrible mistake and just set elevation to zero (!)
        elev = 0 if elev < (-1000000)
      
        # In the future - "db.push_back(Beacon(\"%s\"));\n"
        out.puts [channel, name, callsign, vor_freq, "%.4f" % lat, "%.4f" % lon, elev].join('|')
      end
    end
  end
end