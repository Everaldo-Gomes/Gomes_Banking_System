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

--support table
create table support (
       id	     serial,
       staff_id      integer	  not null,
       opening_day   date	  not null,	
       message	     varchar(501) not null,
       constraint pk_support_id primary key(id),
       constraint fk_staff_id foreign key(staff_id) references staff(id)
);
