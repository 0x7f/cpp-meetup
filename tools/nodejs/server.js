const assert = require('assert');
const cluster = require('cluster');
const http = require('http');

const ajv = require('ajv');

assert(process.env.BENCHMARK_PORT && process.env.BENCHMARK_THREADS);
const PORT = Number(process.env.BENCHMARK_PORT);
const CONCURRENCY = process.env.BENCHMARK_THREADS;
const SKIP_VALIDATION = process.env.BENCHMARK_SKIP_VALIDATION === '1';

const schema = require('../../share/openrtb-schema_bid-request_v2-3.json');
var validate = ajv().compile(schema);

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

      if (!SKIP_VALIDATION && !validate(bidRequest)) {
        res.statusCode = 400;
        res.end("Invalid json:\n" + JSON.stringify(validate.errors, null, 2));
        return;
      }

      res.end("OK:" + bidRequest.id);
    });
  }).listen(PORT, function(){
    console.log("Server listening on: http://localhost:%s", PORT);
  });
}
