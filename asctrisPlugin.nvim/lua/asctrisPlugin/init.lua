local exec_path = "C:\\Users\\jayan\\Documents\\CPP\\tetris\\tetrisMult.exe"
-- local res = os.execute('"' .. exec_path .. '"')
--
local M = {}
-- print("Welcome to Asctris")
--[=====[
M.setup = function ()
    local function asctris_dispatcher(args)
        local func_name = args.fargs[1]
        if func_name and asctris[func_name] then 
            asctris[func_name]()
        else 
            print("Function ".. func_name .. " is invalid for Asctris")

        end 
    end 
    vim.api.nvim.create_user_command('Asctris', asctris_dispatcher, {nargs = 1})
end
--]=====]
M.run_game = function ()
    -- vim.cmd('term ' .. exec_path)
    -- vim.cmd('startinsert')
    -- local width = vim.o.columns -10
    local width = 12
    -- local height = vim.o.lines -10
    local height = 17
    -- local row = 5
    local row = (vim.o.lines/2) - 6
    -- local col = 5
    local col = (vim.o.columns/2) -8

    local buf = vim.api.nvim_create_buf(false,true)
    local opts = {
        style = 'minimal',
        relative = 'editor',
        width = width,
        height = height,
        row = row, 
        col = col,
    }
    vim.api.nvim_open_win(buf, true, opts)

    vim.fn.termopen(exec_path)

    vim.cmd('startinsert')
end 
M.setup = function ()
    local function asctris_dispatcher(args)
        local func_name = args.fargs[1]
        if func_name and asctris[func_name] then 
            asctris[func_name]()
        else 
            print("Function ".. func_name .. " is invalid for Asctris")
        end 
    end 
    vim.api.nvim.create_user_command('Asctris',asctris_dispatcher,{nargs = 1})
end
--[[return {
    run_game = run_game 
}]]--
return M
