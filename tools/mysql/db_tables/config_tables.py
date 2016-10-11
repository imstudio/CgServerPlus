cargo_config = [
    ("cargo_id", "int not null primary key"),
    ("price", "bigint not null"),
    ("item_type", "tinyint default 1"),
    ("item_num", "bigint not null"),
    ("extra", "int default 0"),
    ("sales_type", "tinyint default 0"),
    ("vip_point", "bigint default 0"),
    ("display", 'bool default "1"')
]

cj_config = [
    ('aiid','INT AUTO_INCREMENT PRIMARY KEY'),
    ('target', 'BIGINT NOT NULL'),
    ('reward', 'BIGINT DEFAULT 0'),
    ('reward_type', 'INT DEFAULT 0'),
    ('type', 'INT NOT NULL')
]

vip_config = [
    ('level', 'INT NOT NULL PRIMARY KEY'),
    ('exp_ext', 'INT NOT NULL'),
    ('bounus_ext', 'INT NOT NULL'),
    ('login_ext', 'INT NOT NULL'),
    ('exp_need', 'INT NOT NULL'),
    ('max_bet', 'INT NOT NULL')
]

bet_exp_cfg = [
    ('bet', 'BIGINT NOT NULL PRIMARY KEY'),
    ('exp', 'BIGINT NOT NULL')
]

line_config = [
    ('id', 'INT AUTO_INCREMENT PRIMARY KEY'),
    ('line', 'CHAR(64) NOT NULL') # 0,0,0,0,0
]

grid_config = [
    ('row', 'INT NOT NULL'),
    ('col', 'INT NOT NULL'),
    ('ele_id', 'INT NOT NULL'),
    ('weight', 'INT NOT NULL'),
    ('', 'PRIMARY KEY(row, col, ele_id)')
]

free_grid_config = grid_config

ele_info = [
    ('id', 'INT NOT NULL PRIMARY KEY'),
    ('type', 'TINYINT DEFAULT 0'),
    ('repeatable', 'BOOL NOT NULL')
]

ele_config = [
    ('ele_id', 'INT NOT NULL'),
    ('line_num', 'INT NOT NULL'),
    ('value', 'INT NOT NULL'),
    ('', 'PRIMARY KEY(ele_id, line_num)')
]

theme_common_cfg = [
    ('code', 'INT NOT NULL PRIMARY KEY'),
    ('value', 'BIGINT NOT NULL'),
    ('extra', 'BIGINT DEFAULT 0')
]
