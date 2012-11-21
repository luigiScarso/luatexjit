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
for j=1,1000 do 
 _t[#_t+1]=text
end
text = table.concat(_t)
t_t=nil
--print(#text)

_sf=string.format

local ffi = require("ffi")



--
-- Let's suppose the we already know the size of the lua table array 
-- so we have the faster script to use as reference
-- See dejavuserif-jit-linkedlist.lua on how to remedy to this.
size = 3399

ffi.cdef( _sf([[
struct description { 
   int code_number ;
   /*   */
   int anchors ;
   int boundingbox[4];
   const char *class;
   int  index ;
   const char *name ; 
   int width ;
} ;



typedef struct { 
  const char *cache_uuid;
  double cache_version;
  struct description descriptions[%s]; 
} font_data;
]],size))




font = ffi.new("font_data")
font.cache_uuid    = ffi.new("char[?]",#_d["cache_uuid"]+1,_d["cache_uuid"])
font.cache_version = ffi.new("double",_d["cache_version"])





i=0
first = true
_hast_table__struct_description={}
for k,v in pairs(_d["descriptions"]) do
  local desc = ffi.new("struct description")
  desc.code_number = k
  _hast_table__struct_description[k]=i
  if v.name  then desc.name = v.name   end
  if v.class then desc.class= v.class  end
  if v.width then desc.width=v.width   end
  if v.index then desc.index=v.index   end

  if v.boundingbox and type(v.boundingbox)=='table' then 
    desc.boundingbox[0] = v.boundingbox[1]; desc.boundingbox[1] = v.boundingbox[2]; 
    desc.boundingbox[2] = v.boundingbox[3]; desc.boundingbox[3] = v.boundingbox[4]; 
  end
  if v.boundingbox and type(v.boundingbox)=='number' then 
    desc.boundingbox[0] = v.boundingbox; desc.boundingbox[1] = v.boundingbox; 
    desc.boundingbox[2] = v.boundingbox; desc.boundingbox[3] = v.boundingbox; 
  end
  font.descriptions[i] = desc
  i=i+1
  if i>size then  break end
end
_d = nil


local function get_code_number(desc,n)
  local flag =true
  local p=desc
  for i=0,3399-1 do
    if desc[i].code_number==n then return i end
  end
  return false 
end

local function get_code_number(desc,n)
 return _hast_table__struct_description[n] or false
end


text_p =  ffi.new("char[?]",1+#text,text)
newline = string.byte("\n")
L=100
for i=1,L do 
line_len,line_num =0,1
max = -1 
for j=1,#text do 
 local s = get_code_number(font.descriptions,text_p[j-1])
 --print(s)
 if not(s) then s={['width']=0} else s = font.descriptions[s] end
 if text_p[j-1] == newline then 
  if line_len>max then max = line_len end
  line_num ,line_len = line_num+1 ,0
 else
  line_len = line_len +s.width
 end
end 
end

io.write("max=",max,"\n")
io.write("time:",os.clock()-oldtime,"\n")
io.write("mem:",collectgarbage('count')," KBytes\n")

_hast_table__struct_description=nil




