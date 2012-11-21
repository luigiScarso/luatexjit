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

local ffi = require("ffi")

ffi.cdef[[
typedef struct description *desc_ptr;  

struct  description { 
   desc_ptr prev,next ;
   int code_number ;
   /*   */
   int anchors ;
   int boundingbox[4];
   const char *class;
   int  index ;
   const char *name ; 
   int width ;
}  ;

typedef struct { 
  const char *cache_uuid;
  double cache_version;
  struct description *descriptions; 
} font_data;
]]


font = ffi.new("font_data")

font.cache_uuid    = _d["cache_uuid"]
font.cache_version = _d["cache_version"]


_global_store_struct_description={}
_helper_table_descriptions={}
i=0
base_font_description = nil
prev_font_description = nil
desc_type= ffi.typeof("struct description")
for k,v in pairs(_d["descriptions"]) do
  local desc = desc_type() -- ffi.new("struct description")
  -- need to store in a global table to avoid garbage collection
  -- http://luajit.org/ext_ffi_semantics.html
  -- ffi.cdef[[
  --  typedef struct { int *a; } foo_t;
  --   ]]
  -- 
  -- local s = ffi.new("foo_t", ffi.new("int[10]")) -- WRONG!
  -- 
  -- local a = ffi.new("int[10]") -- OK
  -- local s = ffi.new("foo_t", a)
  -- -- Now do something with 's', but keep 'a' alive until you're done.
  _global_store_struct_description[#_global_store_struct_description+1]=desc

  desc.code_number = k
  _helper_table_descriptions[k]=i  --print(k,i,v.name)
  if v.name  then desc.name = v.name     end
  if v.class then desc.class= v.class    end
  if v.width then desc.width=v.width     end
  if v.index then desc.index=v.index     end

  if v.boundingbox and type(v.boundingbox)=='table' then 
    desc.boundingbox[0] = v.boundingbox[1]; desc.boundingbox[1] = v.boundingbox[2]; 
    desc.boundingbox[2] = v.boundingbox[3]; desc.boundingbox[3] = v.boundingbox[4]; 
  end
  if v.boundingbox and type(v.boundingbox)=='number' then 
    desc.boundingbox[0] = v.boundingbox; desc.boundingbox[1] = v.boundingbox; 
    desc.boundingbox[2] = v.boundingbox; desc.boundingbox[3] = v.boundingbox; 
  end
  if i==0 then
    font.descriptions = desc
    prev_font_description = font.descriptions
  else 
     prev_font_description.next =  desc 
     prev_font_description.next.prev = prev_font_description
     prev_font_description = desc 
  end   
  i=i+1  
end
--_global_store_struct_description=nil => segmentation fault 
--print(i)
size = i
current_ptr = font.descriptions
desc_array=ffi.new("struct description[?]",size)
for j=0,size-1 do
 desc_array[j] = current_ptr[0]
 current_ptr=current_ptr.next
end
_global_store_struct_description=nil
font.descriptions=desc_array


local function get_code_number(desc,k)
  local p=desc
  local i=_helper_table_descriptions[k]
  if i == nil then return false end
  return p[i] 
end



text_p =  ffi.new("char[?]",1+#text,text)
newline = string.byte("\n")
L=100
for i=1,L do
line_len,line_num =0,1
max=-1
for j=1,#text do 
 local s = get_code_number(font.descriptions,text_p[j-1])
 if not(s) then s={['width']=0} end
 if text_p[j-1] == newline then 
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


