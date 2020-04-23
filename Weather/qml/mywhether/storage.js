//storage.js
// 程序打开时，初始化表
function initialize() {
    var db = LocalStorage.openDatabaseSync("mywhether", "1.0", "StorageDatabase", 100000);
    db.transaction(
        function(tx) {
            // 如果setting表不存在，则创建一个
            // 如果表存在，则跳过此步
            tx.executeSql('CREATE TABLE IF NOT EXISTS settings(setting TEXT UNIQUE, value TEXT)');
            //tx.executeSql('INSERT INTO settings  VALUES("default", "101010100")');
      });
}

// 插入数据
function setSetting(setting, value) {
   var db = LocalStorage.openDatabaseSync("mywhether", "1.0", "StorageDatabase", 100000);
   var res = "";
   db.transaction(function(tx) {
        var rs = tx.executeSql('INSERT OR REPLACE INTO settings VALUES (?,?);', [setting,value]);
              //console.log(rs.rowsAffected)
              if (rs.rowsAffected > 0) {
                res = "OK";
              } else {
                res = "Error";
              }
        }
  );
  return res;
}

 // 获取数据
function getSetting(setting) {
   var db = LocalStorage.openDatabaseSync("mywhether", "1.0", "StorageDatabase", 100000);
   var res="";
   db.transaction(function(tx) {
     var rs = tx.executeSql('SELECT value FROM settings WHERE setting=?;', [setting]);
     if (rs.rows.length > 0) {
          res = rs.rows.item(0).value;
     } else {
         res = "Unknown";
     }
  })
  return res
}

//判断是否已有default
function judge(){

    var db = LocalStorage.openDatabaseSync("mywhether", "1.0", "StorageDatabase", 100000);
    db.transaction(function(tx) {
    var rs = tx.executeSql('SELECT value FROM settings WHERE setting="default";');
        console.log(rs.rows.item(0).value);
    if (rs.rows.item(0).value.length > 0) {
        return "true";
    } else {
        return "false";
    }
    })
}
