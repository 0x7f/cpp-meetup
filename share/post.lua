function readWholeFile(file)
    local f = io.open(file, "rb")
    local content = f:read("*all")
    f:close()
    return content
end

wrk.method = "POST"
wrk.headers["Content-Type"] = "application/json"
wrk.body = readWholeFile("share/bidrequest.2.json")
