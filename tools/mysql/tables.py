cargo_info = [
    ("cargo_id", "int not null primary key"),
    ("price", "bigint not null"),
    ("item_type", "tinyint default 1"),
    ("item_num", "bigint not null"),
    ("extra", "int default 0"),
    ("sales_type", "tinyint default 0"),
    ("vip_point", "bigint default 0"),
    ("display", 'bool default "1"')
]

f_history = [
    ("uid", "INT NOT NULL PRIMARY KEY"),
    ("invite_count", "int NOT NULL DEFAULT 0"),
    ("reward_total", "BIGINT DEFAULT 0"),
    ("reward_remain", "BIGINT DEFAULT 0")
]

user_info = [
    ('uid', 'INT AUTO_INCREMENT PRIMARY KEY'),
    ('mid', 'CHAR(48) NOT NULL'),
    ('fname', 'CHAR(24)'),
    ('lname', 'CHAR(24)'),
    ('avatar', 'INT DEFAULT 0'),
    ('male', 'BOOL DEFAULT 1'),
    ('country', 'INT DEFAULT 86'),
]

user_resource = [
    ('uid', 'INT NOT NULL PRIMARY KEY'),
    ('level', 'INT DEFAULT 1'),
    ('exp', 'BIGINT DEFAULT 0'),
    ('fortune', 'BIGINT DEFAULT 0'),
    ('vip_level', 'INT DEFAULT 0'),
    ('vip_point', 'BIGINT DEFAULT 0'),
    ('vip_tmp', 'INT DEFAULT 0'),
    ('vip_tmp_end', 'BIGINT DEFAULT 0')
]

history = [
    ('uid', 'INT NOT NULL'),
    ('max_fortune', 'BIGINT DEFAULT 0'),
    ('max_earned', 'BIGINT DEFAULT 0'),
    ('total_earned', 'BIGINT DEFAULT 0'),
    ('total_bet', 'BIGINT DEFAULT 0'),
    ('tw_earned', 'BIGINT DEFAULT 0'),
    ('lw_earned_sort', 'INT DEFAULT 999'),
    ('lw_level_sort', 'INT DEFAULT 9999'),
    ('lw_fortune_sort', 'INT DEFAULT 9999'),
    ('lw_acheivement_sort', 'INT DEFAULT 9999')
]

# table below is used for game config
vip_config = [
    ('level', 'INT NOT NULL PRIMARY KEY'),
    ('exp_ext', 'INT NOT NULL'),
    ('bounus_ext', 'INT NOT NULL'),
    ('login_ext', 'INT NOT NULL'),
    ('exp_need', 'INT NOT NULL'),
    ('max_bet', 'INT NOT NULL')
]

level_config = [
    ('level', 'INT NOT NULL PRIMARY KEY'),
    ('exp_need', 'BIGINT NOT NULL'), # exp needed to level up
    ('max_bet', 'BIGINT NOT NULL'), # each level can use bet
    ('fortune_reward', 'BIGINT NOT NULL')
]

bet_exp_config = [
    ('bet', 'BIGINT NOT NULL PRIMARY KEY'),
    ('exp', 'BIGINT NOT NULL')
]

line_config = [
    ('id', 'INT AUTO_INCREMENT PRIMARY KEY'),
    ('line', 'CHAR(64) NOT NULL') # 0,0,0,0,0
]

grid_config = [
    ('id', 'INT AUTO_INCREMENT PRIMARY KEY'),
    ('grid_idx', 'INT NOT NULL'),
    ('ele_id', 'INT NOT NULL'),
    ('weight', 'INT NOT NULL')
]

ele_info = [
    ('id', 'INT NOT NULL PRIMARY KEY'),
    ('type', 'TINYINT DEFAULT 0')
]

ele_config = [
    ('_id', 'INT AUTO_INCREMENT PRIMARY KEY'),
    ('ele_id', 'INT NOT NULL'),
    ('line_num', 'INT NOT NULL'),
    ('value', 'INT NOT NULL')
]

table_defines = {
    "cargo_info": cargo_info,
    "vip_config": vip_config,
    'level_config': level_config,
    'bet_exp_cfg': bet_exp_config,
    'line_config': line_config,
    'grid_config': grid_config,
    'ele_info': ele_info,
    'ele_config': ele_config
}

def parse_dict_to_db_sql(table_name, table_define = []):
    sql_tpl = "CREATE TABLE %s (%s)"
    col_sql = ", ".join([t[0] + " " + t[1] for t in table_define])
    table_sql = sql_tpl % (table_name, col_sql)
    return table_sql
