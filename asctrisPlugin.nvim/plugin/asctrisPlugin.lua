local asctris = require('asctrisPlugin')
local function asctris_dispatcher(args)
    local func_name = args.fargs[1]
    if func_name and asctris[func_name] then 
        asctris[func_name]()
    else 
        print("Function ".. func_name .. " is invalid for Asctris")
    end 
end 
vim.api.nvim_create_user_command('Asctris',asctris_dispatcher,{nargs = 1})
