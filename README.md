### Reproduce the Hang

After logging in to ALCF Aurora, clone and build `gslib`:

```sh
git clone https://github.com/Nek5000/gslib.git gslib.git
cd gslib.git
CC=mpicc FC=mpif77 make install
cd ..
```

Then, clone this repository:

```sh
git clone https://github.com/thilinarmtb/nekrs_hang_on_aurora
cd nekrs_hang_on_aurora
```

Copy the `con.co2` file which contains the input for which the `gs()` function
hangs (need to be a member of the `performance` project):

```sh
cp /lus/flare/projects/Performance/thilina/nekrs_hang/helium_water/con.co2 .
```

Then build and submit the `hang` executable:

```sh
CC=mpicc make submit
```
