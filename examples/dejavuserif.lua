oldtime=os.clock()
_d = dofile("dejavuserif.tma")
text = [[We thrive in information||thick worlds because of our
marvelous and everyday capacity to select, edit,
single out, structure, highlight, group, pair, merge,
harmonize, synthesize, focus, organize, condense,
reduce, boil down, choose, categorize, catalog, classify,
list, abstract, scan, look into, idealize, isolate,
discriminate, distinguish, screen, pigeonhole, pick over,
sort, integrate, blend, inspect, filter, lump, skip,
smooth, chunk, average, approximate, cluster, aggregate,
outline, summarize, itemize, review, dip into,
flip through, browse, glance into, leaf through, skim,
refine, enumerate, glean, synopsize, winnow the wheat
from the chaff and separate the sheep from the goats.
]]
_t={}
for j=1,1000 do _t[#_t+1]=text end
text = table.concat(_t)
_t=nil


newline = string.byte("\n")


L=100
for i=1,L do 
line_len,line_num =0,1
max=-1
for j=1,#text do 
 local i = string.byte(text,j)
 local s = _d["descriptions"][i]
 
 if not(s) then s={['width']=0} end
 if i == newline then 
   -- print(line_num,line_len)
  if line_len>max then max =line_len end
  line_num ,line_len = line_num+1 ,0
 else
  line_len = line_len +s.width
 end
end 
end
io.write("max=",max,"\n")


io.write("time:",os.clock()-oldtime,"\n")
io.write("mem:",collectgarbage('count')," KBytes\n")


