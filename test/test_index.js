const { createObject } = require("../index.js");
const assert = require("assert");
const path = require("path")

assert(createObject, "预测试的功能未定义-add");

function testBasic() {
    let p;
    if (process.platform === 'darwin') {
        p = path.join(__dirname, '../测试用dll/libmacSdk.dylib');
    } else {
        p = path.join(__dirname, "../测试用dll/demoDll.dll");
    }

    // const p = "/Users/qilv/Desktop/work/code/cplus/test/libmacSdk.dylib";
    const result = createObject(p).add(1111, 222);
    assert.strictEqual(result, 1333, `返回了其他值${result}`);
}

assert.doesNotThrow(testBasic, undefined, `testBasic 抛出异常`);

console.log("测试通过!")