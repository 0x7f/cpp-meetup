const assert = require('assert');
const cluster = require('cluster');
const http = require('http');

assert(process.env.BENCHMARK_THREADS && process.env.BENCHMARK_PORT);
const CONCURRENCY = process.env.BENCHMARK_THREADS;
const PORT = Number(process.env.BENCHMARK_PORT);

if (cluster.isMaster) {
  for (var i = 0; i < CONCURRENCY; i++) {
    cluster.fork();
  }
  cluster.on('exit', function(worker, code, signal) {
    console.log("worker", worker.process.pid, "died");
  });
} else {
  http.createServer(function(req, res) {
    if (req.url != "/add" && req.method != 'POST') {
      res.statusCode = 404;
      res.end();
      return;
    }

    var body = "";
    req.on('data', function(data) {
      body += data;
    });
    req.on('end', function() {
      var bidRequest = JSON.parse(body);
      res.end("OK");
    });
  }).listen(PORT, function(){
    console.log("Server listening on: http://localhost:%s", PORT);
  });
}


