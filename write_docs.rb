#!/usr/bin/ruby
$KCODE = 'u'
require 'rubygems'
require 'rdiscount'
require 'fileutils'

def format_table(file_contents)
  # 39|DZHAMBUL|MH|117.90|42.8667|71.2833|0
  file_contents.split("\n").map do | line |
    channel, name, callsign, vor, lat, lon, elev = line.split("|")
    [name, channel, callsign, fmt_degrees(lat), fmt_degrees(lon)].map{|e| "<td>%s</td>" % e }.join
  end.map{|ln| "<tr>%s</tr>" % ln }.join("\n")
end

def fmt_degrees(deg_decimal)
  d = deg_decimal.to_f
  rem = ((d - d.to_i) * 60).to_i
  "%d&#176;%d&#8242;" % [d, rem]
end

def format_beacon_list(file_contents)
  t = format_table(file_contents)
  "<table><tr><th>Beacon</th><th>Channel</th><th>Callsign</th><th>Lat</th><th>Lon</th></tr>#{t}</table>"
end

class FH
  TEMPLATE = DATA.read
  @@files = []
  
  def initialize(path)
    @path = path
    @table = path.include?(".dat")
    @@files << self
  end
  
  def html_name
    File.basename(@path).gsub(/\s|\//, '_').gsub(/\.(.+)$/, '.html')
  end
  
  def file_name
    File.basename(@path)
  end
  
  def to_s
    content = File.read(@path)
    content = format_beacon_list(content) if @table
    TEMPLATE % [navbar, RDiscount.new(content).to_html]
  end
  
  def navbar
    @@files.map do | fh |
      "<li><a href='%s'>%s</a></li>" % [fh.html_name, fh.file_name]
    end
  end
end

dist = "rsbn"
FileUtils.mkdir_p("#{dist}/data")

%w( README.txt data/ussr.dat data/cis.dat ).map{|f| FH.new(f) }.each do | doc_file |
  d = "#{dist}/" + doc_file.html_name
  puts d
  File.open(d, "w") do | f |
      f << doc_file.to_s
  end
end

%w( mac.xpl win.xpl lin.xpl).each do | plug |
  FileUtils.cp(plug, "#{dist}/") if File.exist?(plug)
end

Dir.glob("data/*.dat").each do | dat_file |
  FileUtils.cp(dat_file, "#{dist}/data/")
end

__END__
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml1-transitional.dtd">
<html lang="en" xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
  <head>
    <meta content="text/html; charset=utf-8" http-equiv="Content-Type" />
    <title>RSBN plugin</title>
    <style type='text/css'>
      body { font-family: Georgia, sans-serif; font-size: .9em; }
      table { font-family: monospace; font-size: .9em; }
    </style>
  </head>
  <body>
    <!-- nav -->
    <ul class="navbar">%s</ul>
    <!-- content -->
    %s
  </body>
</html>