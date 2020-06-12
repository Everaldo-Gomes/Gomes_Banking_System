--staff table
create table staff (
      id	   serial, 
      full_name    varchar(100) not null,
      cpf          varchar(11) 	not null,
      address      varchar(50) 	not null,
      phone_number varchar(14) 	not null,
      birthday     date		not null,
      email 	   varchar(50) 	not null,
      sector 	   varchar(10) 	not null,
      password 	   varchar(10) 	not null,
      created_in   date 	not null,
      constraint pk_id primary key(id)
);
