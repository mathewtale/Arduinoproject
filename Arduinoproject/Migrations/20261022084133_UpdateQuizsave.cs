using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace Arduinoproject.Migrations
{
    /// <inheritdoc />
    public partial class UpdateQuizsave : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "Answers",
                table: "Quizsave");

            migrationBuilder.RenameColumn(
                name: "Catagory",
                table: "Quizsave",
                newName: "Category");

            migrationBuilder.AddColumn<string>(
                name: "Answer1",
                table: "Quizsave",
                type: "text",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<string>(
                name: "Answer2",
                table: "Quizsave",
                type: "text",
                nullable: false,
                defaultValue: "");

            migrationBuilder.AddColumn<string>(
                name: "Answer3",
                table: "Quizsave",
                type: "text",
                nullable: false,
                defaultValue: "");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropColumn(
                name: "Answer1",
                table: "Quizsave");

            migrationBuilder.DropColumn(
                name: "Answer2",
                table: "Quizsave");

            migrationBuilder.DropColumn(
                name: "Answer3",
                table: "Quizsave");

            migrationBuilder.RenameColumn(
                name: "Category",
                table: "Quizsave",
                newName: "Catagory");

            migrationBuilder.AddColumn<string>(
                name: "Answers",
                table: "Quizsave",
                type: "text",
                nullable: true);
        }
    }
}
