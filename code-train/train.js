const brain = require('brain.js');
const trainData = require("./hu-train.json");


let net;

const nnConfig = {
  hiddenLayers: [20, 50, 20],
};

function log(msg) {
  console.log(msg);
}

async function run() {
  log("Training");
  net = new brain.NeuralNetworkGPU(nnConfig);
  await net.trainAsync(trainData, {
    iterations: 300,
    logPeriod: 10,
    log: (details) => {
      log("Iter #" + details.iterations + "; error: " + details.error);
    },
  });
}

run();
